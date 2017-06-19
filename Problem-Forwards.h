/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: Using BSF Skeleton for NSLP Algorithm Implementation
Module: Problem-Forwards.h (BSF Function Forwards)
Author: Irina M. Sokolinskaya, Leonid B. Sokolinsky
Creation Date: 03.05.2017
==============================================================================*/
#pragma once


static void PI_MapF(int cohortIndex);					// Worker Process
static bool PI_ExitF();									// Function Calculating Exit Condition
static void PI_OutputF();								// Output Function

static void PI_Init();									// Problem Initialization
static void DataInit(); // Problem Data Initialization
static bool PointInM(PT_point_T point); // If point belonges to M?
static double ObjectiveF(PT_point_T point); // Calculating Value of Objective Function in Point

