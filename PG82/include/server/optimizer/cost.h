/*-------------------------------------------------------------------------
 *
 * cost.h
 *	  prototypes for costsize.c and clausesel.c.
 *
 *
 * Portions Copyright (c) 1996-2006, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/optimizer/cost.h,v 1.82 2006/10/04 00:30:09 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef COST_H
#define COST_H

#include "nodes/plannodes.h"
#include "nodes/relation.h"


/* defaults for costsize.c's Cost parameters */
/* NB: cost-estimation code should use the variables, not these constants! */
/* If you change these, update backend/utils/misc/postgresql.sample.conf */
#define DEFAULT_SEQ_PAGE_COST  1.0
#define DEFAULT_RANDOM_PAGE_COST  4.0
#define DEFAULT_CPU_TUPLE_COST	0.01
#define DEFAULT_CPU_INDEX_TUPLE_COST 0.005
#define DEFAULT_CPU_OPERATOR_COST  0.0025

#define DEFAULT_EFFECTIVE_CACHE_SIZE  16384		/* measured in pages */


/*
 * prototypes for costsize.c
 *	  routines to compute costs and sizes
 */

/* parameter variables and flags */
extern DLLIMPORT double seq_page_cost;
extern DLLIMPORT double random_page_cost;
extern DLLIMPORT double cpu_tuple_cost;
extern DLLIMPORT double cpu_index_tuple_cost;
extern DLLIMPORT double cpu_operator_cost;
extern DLLIMPORT int effective_cache_size;
extern Cost disable_cost;
extern bool enable_seqscan;
extern bool enable_indexscan;
extern bool enable_bitmapscan;
extern bool enable_tidscan;
extern bool enable_sort;
extern bool enable_hashagg;
extern bool enable_nestloop;
extern bool enable_mergejoin;
extern bool enable_hashjoin;
extern bool constraint_exclusion;

extern double clamp_row_est(double nrows);
extern double index_pages_fetched(double tuples_fetched, BlockNumber pages,
					double index_pages, PlannerInfo *root);
extern void cost_seqscan(Path *path, PlannerInfo *root, RelOptInfo *baserel);
extern void cost_index(IndexPath *path, PlannerInfo *root, IndexOptInfo *index,
		   List *indexQuals, RelOptInfo *outer_rel);
extern void cost_bitmap_heap_scan(Path *path, PlannerInfo *root, RelOptInfo *baserel,
					  Path *bitmapqual, RelOptInfo *outer_rel);
extern void cost_bitmap_and_node(BitmapAndPath *path, PlannerInfo *root);
extern void cost_bitmap_or_node(BitmapOrPath *path, PlannerInfo *root);
extern void cost_bitmap_tree_node(Path *path, Cost *cost, Selectivity *selec);
extern void cost_tidscan(Path *path, PlannerInfo *root,
			 RelOptInfo *baserel, List *tidquals);
extern void cost_subqueryscan(Path *path, RelOptInfo *baserel);
extern void cost_functionscan(Path *path, PlannerInfo *root,
				  RelOptInfo *baserel);
extern void cost_valuesscan(Path *path, PlannerInfo *root,
				RelOptInfo *baserel);
extern void cost_sort(Path *path, PlannerInfo *root,
		  List *pathkeys, Cost input_cost, double tuples, int width);
extern void cost_material(Path *path,
			  Cost input_cost, double tuples, int width);
extern void cost_agg(Path *path, PlannerInfo *root,
		 AggStrategy aggstrategy, int numAggs,
		 int numGroupCols, double numGroups,
		 Cost input_startup_cost, Cost input_total_cost,
		 double input_tuples);
extern void cost_group(Path *path, PlannerInfo *root,
		   int numGroupCols, double numGroups,
		   Cost input_startup_cost, Cost input_total_cost,
		   double input_tuples);
extern void cost_nestloop(NestPath *path, PlannerInfo *root);
extern void cost_mergejoin(MergePath *path, PlannerInfo *root);
extern void cost_hashjoin(HashPath *path, PlannerInfo *root);
extern void cost_qual_eval(QualCost *cost, List *quals);
extern void set_baserel_size_estimates(PlannerInfo *root, RelOptInfo *rel);
extern void set_joinrel_size_estimates(PlannerInfo *root, RelOptInfo *rel,
						   RelOptInfo *outer_rel,
						   RelOptInfo *inner_rel,
						   JoinType jointype,
						   List *restrictlist);
extern void set_function_size_estimates(PlannerInfo *root, RelOptInfo *rel);
extern void set_values_size_estimates(PlannerInfo *root, RelOptInfo *rel);

/*
 * prototypes for clausesel.c
 *	  routines to compute clause selectivities
 */
extern Selectivity clauselist_selectivity(PlannerInfo *root,
					   List *clauses,
					   int varRelid,
					   JoinType jointype);
extern Selectivity clause_selectivity(PlannerInfo *root,
				   Node *clause,
				   int varRelid,
				   JoinType jointype);

#endif   /* COST_H */
