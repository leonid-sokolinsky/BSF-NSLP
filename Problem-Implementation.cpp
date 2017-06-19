/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: Using BSF Skeleton for NSLP Algorithm Implementation
Module: Problem-Implementation.h (Implementation of the Problem)
Prefix: PI
Author: Irina M. Sokolinskaya, Leonid B. Sokolinsky
Creation Date: 03.06.2017
==============================================================================*/
#include "BSF-Include.h"
#include "Problem-Data.h"			// Problem Data 
#include "Problem-Forwards.h"		// Function Forwards
using namespace std;

static void PI_MapF(int cohortIndex) {
#define SHIFT(k) (k < PP_K / 2?PP_K / 2 + k - PP_K:k - PP_K / 2 + 1) // Shift relative to center
	PT_point_T point;	// Current point
	double objectiveCenter = ObjectiveF(PD_center); // Value of Objective Function in Central Point
	double objectiveMax; // Maximum of Objective Function on Cohort
	double objectiveCurrent; // Value of Objective Function in Current Point
	int kMax;	// Number of Point having Maximum of Objective Function

	objectiveMax = -DBL_MAX;
	kMax = -1;
	for (int k = 0; k < PP_K; k++) { // k: Number of Point in Cohort

		for (int j = 0; j < PP_N; j++) // j: Number of Point Coordinate
			point[j] = PD_center[j];
		point[cohortIndex] += PD_s*SHIFT(k);

		if (!PointInM(point)) continue;

		objectiveCurrent = ObjectiveF(point);
		if (objectiveCurrent <= objectiveCenter || objectiveCurrent <= objectiveMax) continue;
		objectiveMax = objectiveCurrent;
		kMax = k;
	};
	if (kMax < 0) {
		PD_report[cohortIndex].count = 0;
		return;
	};

	PD_report[cohortIndex].count = 1;
	for (int j = 0; j < PP_N; j++)
		PD_report[cohortIndex].vect[j] = PD_center[j];
	PD_report[cohortIndex].vect[cohortIndex] += PD_s*SHIFT(kMax);
	//cout << PD_rank << ": vect= "; 	for (int j = 0; j < PP_N; j++) 	cout << PD_report[cohortIndex].vect[j] << "\t"; cout << endl;
};

static void PI_ReduceF(PT_report_T* reportX, PT_report_T* reportY, PT_report_T* reportZ) { // reportZ = reportX + reportY
	for (int j = 0; j < PP_N; j++)
		reportZ->vect[j] = reportX->vect[j] * BM_SGN(reportX->count) + reportY->vect[j] * BM_SGN(reportY->count);
	reportZ->count = reportX->count + reportY->count;
};

static bool PI_ExitF() { // Function calculating exit condition
	static int counter = 0;	// Iteration Counter
	counter++;
	if (counter < PP_ITER_COUNT)
		return false;
	else
		return true;
};

static void PI_Init() { // Problem Initialization
	MPI_Comm_rank(MPI_COMM_WORLD, &PD_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &PD_size);

	PD_report = (PT_report_T*)malloc((PP_N) * sizeof(PT_report_T));
	DataInit();
};

static void DataInit() { // Problem Data Initialization

	for (int j = 0; j < PP_DIM; j++) // Generating Cohot Numbers
		PD_list[j] = j;

	for (int j = 0; j < PP_N; j++) // Generating Coordinates of Cross Center
		PD_center[j] = PP_SF / 2;
	PD_s = PP_S;
	//	cout << PD_rank << ": Cross Center="  << PD_center[0] << "\t" << PD_center[1] << endl;

	for (int j = 0; j < PP_N; j++) // Generating Objective Function Coefficients
		PD_c[j] = 2;
	PD_c[0] = 1;

	for (int j = 0; j < PP_N; j++) { // Generating Matrix À (including -x_j<=0)
		PD_A[j][j] = 1;
		PD_A[PP_N][j] = 1;
		PD_A[PP_N + 1][j] = -1;
		PD_A[j + PP_N + 2][j] = -1;
	};

	for (int i = 0; i < PP_N; i++) // Generating column b (including -x_j<=0)
		PD_b[i] = PP_SF;
	PD_b[PP_N] = PP_SF*(PP_N - 1) + PP_SF / 2;
	PD_b[PP_N + 1] = -PP_SF / 2;
	for (int i = PP_N + 2; i < 2 * PP_N + 2; i++)
		PD_b[i] = 0;
};

static bool PointInM(PT_point_T point) { // If point belonges to M?
	double sum;
	for (int i = 0; i < PP_M; i++) {
		sum = 0;
		for (int j = 0; j < PP_N; j++)
			sum += PD_A[i][j] * point[j];
		if (sum > PD_b[i])
			return false;
	};
	return true;
};

static double ObjectiveF(PT_point_T point) { // Calculating Value of Objective Function in Point
	double res = 0;
	for (int j = 0; j < PP_N; j++)
		res += PD_c[j] * point[j];
	return res;
};

static void PI_OutputF() {// Output Function
/*	cout << "vect = ";
	for (int j = 0; j < PP_N; j++)
		cout << PD_report[0].vect[j] << "\t";
	cout << endl;
	cout << "count = " << PD_report[0].count << endl;/**/
};
