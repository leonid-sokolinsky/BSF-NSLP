/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: Using BSF Skeleton for NSLP Algorithm Implementation
Module: Problem-Types.h (BSF Types)
Prefix: PT
Author: Irina M. Sokolinskaya, Leonid B. Sokolinsky
Creation Date: 30.05.2017
==============================================================================*/
#pragma once
#include "BSF-Include.h"

//=========================== Problem Types =========================
typedef double PT_point_T[PP_N];	// Point in n-Dimensional Space
								
//=========================== BSF Types =========================
typedef int PT_elem_T;	// Type of List Element - Cohort Number

struct PT_report_T { // Type of Result Element
	PT_point_T vect;	// Vector of Sum of Points
	double count;		// Count of Points
};

