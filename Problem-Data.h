/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: Using BSF Skeleton for NSLP Algorithm Implementation
Module: Problem-Data.h (Problem Data)
Author: Irina M. Sokolinskaya, Leonid B. Sokolinsky
Creation Date: 30.05.2017
==============================================================================*/
#pragma once
#include "BSF-Include.h"

//========================== Variables ====================================
static double PD_s;			// Distance Between Neighbor Points in Cohort
static int PD_rank;			// Rank of the current process MPI 
static int PD_size;			// Number of MPI processes

//========================== Structures ====================================
static PT_elem_T PD_list[PP_DIM];		// List of Cohot Numbers
static PT_point_T PD_cross[PP_DIM][PP_K];	// Cross[Cohort Number][Entry Number] without central point

static PT_report_T* PD_report;		// List of Reports
static PT_point_T PD_center;		// Central Point of Cross

/* ----------- Linear Programming Problem ----------- */
static double PD_A[PP_M][PP_N];	// Coefficient Matrix
static double PD_b[PP_M];		// Vector of Right Members
static double PD_c[PP_N];		// Objective Function Coefficients