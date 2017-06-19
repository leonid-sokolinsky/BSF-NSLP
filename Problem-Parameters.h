/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: Using BSF Skeleton for NSLP Algorithm Implementation
Module: Problem-Parameters.h (Problem Parameters)
Prefix: PP
Author: Irina M. Sokolinskaya, Leonid B. Sokolinsky
Creation Date: 23.05.2017
==============================================================================*/
#pragma once

/* ========================= Compilation Modes ======================== */
//#define NDEBUG
//#define _CLUSTER

//=========================== Problem Parameters =========================
#define PP_N 800			// Dimension of Solution Space
#define PP_K 6				// Number of Points in Cohort Excluding Central Point
#define PP_M (2*PP_N+2)		// Number of Constraints

#define PP_SF	200			// Scale Factor
#define PP_S	20			// Initial Distance Between Neighbor Points in Cohort

#define PP_ITER_COUNT 100	// Count of Main Loop Iterations
//=========================== BSF Paramrters =========================
//#define PP_DIM			<<Integer > 0>>	// Problem Dimension = Number of Data Elements in List
#define PP_DIM PP_N		// Number of Cohorts = Dimension of Solution Space