/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: BSF Skeleton
Module: BSF-Data.h (BSF Data)
Prefix: BD
Author: Leonid B. Sokolinsky
Creation Date: 30.05.2017
==============================================================================*/
#pragma once
#include "BSF-Include.h"
// ========================= Constants ========================
#define BD_EXIT	true			// End of Work
// ========================= Variables ========================
static int	BD_rank;			// Rank of the current process MPI 
static int	BD_size;			// Number of MPI processes
static int	BD_masterRank;		// Rank of Master
static int	BD_numOfWorkers;	// Number of Workers = Number of MPI processes - 1
static int	BD_elemsPerWorker;	// Number of elements per worker
static int	BD_tailLength;		// Number of elements for last worker

// ========================= Lists ========================
BT_order_T* BD_order;		// Orders for Workers
MPI_Status*  BD_status;
MPI_Request* BD_request;
