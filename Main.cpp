/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: BSF Skeleton
Module: BSF-Implementation.h (Implementation of BSF Skeleton)
Prefix: BI
Author: Leonid B. Sokolinsky
Creation Date: 30.05.2017
==============================================================================*/
#include "BSF-Include.h"
#include "BSF-Data.h"				// Problem Data 
#include "BSF-Forwards.h"			// Function Forwards
#include "BSF-Forwards.h"			// Function Forwards

using namespace std;

#include "Problem-Implementation.cpp"

double startTime, endTime;

int main(int argc, char *argv[]) {
	BI_Init();
	PI_Init();
	if (BD_rank == BD_masterRank)
		BI_Master();
	else
		BI_Worker();
	MPI_Finalize();
	return 0;
};
static void BI_Master() {// Master Process
//	cout << "MPI size = " << BD_size << endl;
	cout << "n = " << PP_N << endl;
	cout << "Number of workers: " << BD_numOfWorkers << endl;
	cout << "Number of iterations: " << PP_ITER_COUNT << endl;
	startTime = MPI_Wtime();
	do {
		BI_MasterMap(!BD_EXIT);
		BI_MasterReduce();
		BI_MasterAgrigate();
	} while (!PI_ExitF());
	endTime = MPI_Wtime(); 	cout << "Runtime = " << endTime - startTime << endl;
	BI_MasterMap(BD_EXIT);
	PI_OutputF();
};
static void BI_Worker() {// Worker Process
	bool exit;
	while (true) {
		exit = BI_WorkerMap();
		if (exit) break;
		BI_WorkerReduce();
	};
};

static void BI_MasterMap(bool exit) {
#define APPENDIX(tail) (tail>0 ? 1 : 0)	
	int tail = BD_tailLength;
	int index = 0;

	for (int rank = 0; rank < BD_numOfWorkers; rank++) {
		BD_order[rank].exit = exit;
		BD_order[rank].index = index;
		BD_order[rank].length = BD_elemsPerWorker + APPENDIX(tail);
		MPI_Isend(
			&BD_order[rank],
			sizeof(BT_order_T),
			MPI_BYTE,
			rank,
			0,
			MPI_COMM_WORLD,
			&BD_request[rank]);
		index += BD_order[rank].length;
		tail--;
	};
	MPI_Waitall(BD_numOfWorkers, BD_request, BD_status);
};

static void BI_MasterReduce() {
	for (int rank = 0; rank < BD_numOfWorkers; rank++)
		MPI_Irecv(
			&PD_report[rank],
			sizeof(PT_report_T),
			MPI_BYTE,
			rank,
			0,
			MPI_COMM_WORLD,
			&BD_request[rank]);
	MPI_Waitall(BD_numOfWorkers, BD_request, BD_status);
	for (int rank = 1; rank < BD_numOfWorkers; rank++)
		PI_ReduceF(&PD_report[0], &PD_report[rank], &PD_report[0]);
};

static void BI_MasterAgrigate() {
	for (int j = 0; j < PP_N; j++)
		PD_report[0].vect[j] /= PD_report[0].count;
};

static bool BI_WorkerMap() {
	MPI_Recv(
		&BD_order[BD_rank],
		sizeof(BT_order_T),
		MPI_BYTE,
		BD_masterRank,
		0,
		MPI_COMM_WORLD,
		&BD_status[BD_rank]);

	if (BD_order[BD_rank].exit) return BD_EXIT;

	for (int cohortIndex = BD_order[BD_rank].index; cohortIndex < BD_order[BD_rank].index + BD_order[BD_rank].length; cohortIndex++)
		PI_MapF(cohortIndex);

	return !BD_EXIT;
};

static void BI_WorkerReduce() {
	for (int i = BD_order[BD_rank].index + 1; i < BD_order[BD_rank].index + BD_order[BD_rank].length; i++)
		PI_ReduceF(&PD_report[BD_order[BD_rank].index], &PD_report[i], &PD_report[BD_order[BD_rank].index]);

	//cout << BD_rank << ": BI_WorkerReduce: vect= "; 	for (int j = 0; j < PP_N; j++) 	cout << PD_report[BD_order[BD_rank].index].vect[j] << "\t"; cout << endl;
	MPI_Send(
		&PD_report[BD_order[BD_rank].index],
		sizeof(PT_report_T),
		MPI_BYTE,
		BD_masterRank,
		0,
		MPI_COMM_WORLD);
};

static void BI_Init() {// Initialization
	int rc = MPI_Init(NULL, NULL);	// Starting MPI 
	if (rc != MPI_SUCCESS) {
		printf("Error starting MPI program. Terminating! \n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	};
	MPI_Comm_rank(MPI_COMM_WORLD, &BD_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &BD_size);
	if (BD_size < 2) {
		if (BD_rank == 0) cout << "Error: MPI_SIZE must be > 1" << endl;
		MPI_Finalize();
		exit(1);
	};
	if (BD_size > PP_N + 1) {
		if (BD_rank == 0) cout << "Error: MPI_SIZE must be < n+2=" << PP_N + 2 << endl;
		MPI_Finalize();
		exit(1);
	};

	BD_masterRank = BD_size - 1;
	BD_numOfWorkers = BD_size - 1;
	BD_elemsPerWorker = PP_DIM / BD_numOfWorkers;
	assert(BD_elemsPerWorker > 0);
	BD_tailLength = PP_DIM - BD_elemsPerWorker*BD_numOfWorkers;
	BD_status = (MPI_Status*)malloc(BD_numOfWorkers * sizeof(MPI_Status));
	BD_request = (MPI_Request*)malloc(BD_numOfWorkers * sizeof(MPI_Request));
	BD_order = (BT_order_T*)malloc(BD_numOfWorkers * sizeof(BT_order_T));
};