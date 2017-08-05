/**
 * @file reader.h
 * @author Erik Bergsten
 * @date Fri 28 Jul 2017
 * @brief Reader for crisp, reads a parsed crisp object
 *
 * The reader reads keywords and macros and evaluates the results. This phase
 * comes after the tokenizing and parsing of the source code.
 */

#ifndef __crisp_reader_h__
#define __crisp_reader_h__

/**
 * @brief Reads and evaluates parsed crisp statements.
 *
 * The reader transforms and then evaluates the code. This is where
 * macros are evaluated and where module commands/declarations are
 * handled.
 */
void cr_read(cr_object * statement, cr_runtime * runtime);

void cr_reader_init();

#endif /* __crisp_reader_h__ */
