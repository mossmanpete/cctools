/*
Copyright (C) 2018- The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file COPYING for details.
*/

#ifndef BATCH_TASK_H
#define BATCH_TASK_H

#include "batch_file.h"
#include "list.h"
#include "jx.h"
#include "rmsummary.h"

struct batch_task {
	int taskid;                  /* Indicates the id provided by the creating system. I.E. Makeflow */
	int jobid;                   /* Indicates the id assigned to the job by the submission system. */

	struct batch_queue *queue;   /* The queue this task is assigned to. */

	char *command;               /* The command line to execute. */

	struct list   *input_files;  /* Task's required inputs, type batch_file */
	struct list   *output_files; /* Task's expected outputs, type batch_file */

	struct rmsummary *resources; /* Resources assigned to task */

	struct jx *envlist;          /* JSON formatted environment list */ 

	struct batch_job_info *info; /* Stores the info struct created by batch_job. */
};

/** Create a batch_task struct.
@param queue The queue this task is associated with/assigned to.
@return A batch_task struct in newly alloced space.
*/
struct batch_task *batch_task_create(struct batch_queue *queue );

/** Delete a batch_task struct.
 This frees the command, deletes the files and lists, deletes the resources, and deletes envlist.
@param t The batch_task struct to be freed.
*/
void batch_task_delete(struct batch_task *t);

/** Add file to input list of batch_task
 Creates a new batch_file from name_on_submission and name_on_execution.
 This newly created file is add to input_files.
 For clarifications on name_on_submission, name_on_execution, and their uses see batch_file.
@param task The batch_task this file is being added to.
@param name_on_submission The name of the file at submission/host site.
@param name_on_execution The name of the file at execution site.
@return A pointer to the newly allocated batch_file struct.
*/
struct batch_file * batch_task_add_input_file(struct batch_task *task, char * name_on_submission, char * name_on_execution);

/** Add file to output list of batch_task
 Creates a new batch_file from name_on_submission and name_on_execution.
 This newly created file is add to output_files.
 For clarifications on name_on_submission, name_on_execution, and their uses see batch_file.
@param task The batch_task this file is being added to.
@param name_on_submission The name of the file at submission/host site.
@param name_on_execution The name of the file at execution site.
@return A pointer to the newly allocated batch_file struct.
*/
struct batch_file * batch_task_add_output_file(struct batch_task *task, char * name_on_submission, char * name_on_execution);

/** Set the command of the batch_task.
 Frees previous command and xxstrdups new command.
@param t The batch_task to be updated.
@param command The new command to use.
*/
void batch_task_set_command(struct batch_task *t, char *command);

/** Wrap the existing command with a template string.
 This uses string_wrap_command to wrap command, see stringtools.h for details.
 This function allocates a new string with the result and free the previous command.
 Does not free passed command. Will use wrapper interface in future.
@param t The batch_task whose command is being wrapped.
@param command The command template that will wrap existing command.
*/
void batch_task_wrap_command(struct batch_task *t, char *command);

/** Set the resources needed for task.
 This function will make a copy of full copy of resources using rmsummary_copy().
@param t The batch_task requiring specified resources.
@param resources A rmsummary specifying required resources.
*/
void batch_task_set_resources(struct batch_task *t, struct rmsummary *resources);

/** Set the envlist for this task.
 This function will make a copy using jx_copy of envlist.
@param t The batch_task using this environment.
@param envlist The jx_object specifying the environment.
*/
void batch_task_set_envlist(struct batch_task *t, struct jx *envlist);

/** Set the batch_job_info of this task.
 Performs simple copy into already allocated memory.
@param t The batch_task that was completed.
@param info The batch_job_info of the completed task.
*/
void batch_task_set_info(struct batch_task *t, struct batch_job_info *info);

#endif
/* vim: set noexpandtab tabstop=4: */