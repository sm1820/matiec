/*
 * (c) 2003 Mario de Sousa
 *
 * Offered to the public under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * This code is made available on the understanding that it will not be
 * used in safety-critical situations without a full and competent review.
 */

/*
 * An IEC 61131-3 IL and ST compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

/*
 * Function call parameter iterator.
 * It will iterate through the formal parameters of a function call
 * (i.e. function calls using the foo(<param1>, <param2>, ...) syntax).
 * and/or search through the non-formal parameters of a function call
 * (i.e. function calls using the foo(<name1> = <param1>, <name2> = <param2>, ...) syntax).
 *
 * Calls to function blocks and programs are also supported.
 *
 * Note that calls to next() will only iterate through formal parameters,
 * and calls to search()  will only serach through non-formal parameters.
 */


#include "../absyntax/visitor.hh"


class function_call_param_iterator_c : public null_visitor_c {

  private:
      /* a pointer to the function call
       * (or function block or program call!)
       */
    symbol_c *f_call;
    int next_param, param_count;
    identifier_c *search_param_name;

    /* Which operation of the class was called...
     * Search a parameter, or iterate to the next parameter.
     */
    typedef enum {iterate_op, search_op} operation_t;
    operation_t current_operation;

  private:
    void *search_list(list_c *list);
    void *handle_parameter_assignment(symbol_c *variable_name, symbol_c *expression) ;


  public:
    /* start off at the first parameter once again... */
    void reset(void);

    /* initialise the iterator object.
     * We must be given a reference to the function/program/function block call
     * that will be analysed...
     */
    function_call_param_iterator_c(symbol_c *f_call);

    /* Skip to the next parameter. After object creation,
     * the object references on parameter _before_ the first, so
     * this function must be called once to get the object to
     * reference the first parameter...
     *
     * Returns whatever is being passed to the parameter!
     */
    symbol_c *next(void);

    /* Search for the value passed to the parameter named <param_name>...  */
    symbol_c *search(symbol_c *param_name);


  private:
  /********************************/
  /* B 1.7 Configuration elements */
  /********************************/
 
  /*
  CONFIGURATION configuration_name
     optional_global_var_declarations
   (resource_declaration_list | single_resource_declaration)
   optional_access_declarations
   optional_instance_specific_initializations
  END_CONFIGURATION
  */
  /*
  SYM_REF6(configuration_declaration_c, configuration_name, global_var_declarations, resource_declarations, access_declarations, instance_specific_initializations, unused)
  */

  /* helper symbol for configuration_declaration */
  /*
  SYM_LIST(resource_declaration_list_c)
  */

  /*
  RESOURCE resource_name ON resource_type_name
     optional_global_var_declarations
   single_resource_declaration
  END_RESOURCE
  */
  /*
  SYM_REF4(resource_declaration_c, resource_name, resource_type_name, global_var_declarations, resource_declaration)
  */

  /* task_configuration_list program_configuration_list */
  /*
  SYM_REF2(single_resource_declaration_c, task_configuration_list, program_configuration_list)
  */

  /* helper symbol for single_resource_declaration */
  /*
  SYM_LIST(task_configuration_list_c)
  */

  /* helper symbol for single_resource_declaration */
  /*
  SYM_LIST(program_configuration_list_c)
  */

  /* helper symbol for
   *  - access_path
   *  - instance_specific_init
   */
  /*
  SYM_LIST(any_fb_name_list_c)
  */

  /*  [resource_name '.'] global_var_name ['.' structure_element_name] */
  /*
  SYM_REF4(global_var_reference_c, resource_name, global_var_name, structure_element_name, unused)
  */

  /*  prev_declared_program_name '.' symbolic_variable */
  /*
  SYM_REF2(program_output_reference_c, program_name, symbolic_variable)
  */
 
  /*  TASK task_name task_initialization */
  /*
  SYM_REF2(task_configuration_c, task_name, task_initialization)
  */

  /*  '(' [SINGLE ASSIGN data_source ','] [INTERVAL ASSIGN data_source ','] PRIORITY ASSIGN integer ')' */
  /*
  SYM_REF4(task_initialization_c, single_data_source, interval_data_source, priority_data_source, unused)
  */

  /*  PROGRAM [RETAIN | NON_RETAIN] program_name [WITH task_name] ':' program_type_name ['(' prog_conf_elements ')'] */
  // SYM_REF6(program_configuration_c, retain_option, program_name, task_name, program_type_name, prog_conf_elements, unused)
    void *visit(program_configuration_c *symbol);

  /* prog_conf_elements ',' prog_conf_element */
  // SYM_LIST(prog_conf_elements_c)
    void *visit(prog_conf_elements_c *symbol);

  /*  fb_name WITH task_name */
  /*
  SYM_REF2(fb_task_c, fb_name, task_name)
  */

  /*  any_symbolic_variable ASSIGN prog_data_source */
  // SYM_REF2(prog_cnxn_assign_c, symbolic_variable, prog_data_source)
    void *visit(prog_cnxn_assign_c *symbol);

  /* any_symbolic_variable SENDTO data_sink */
  // SYM_REF2(prog_cnxn_sendto_c, symbolic_variable, prog_data_source)
    void *visit(prog_cnxn_sendto_c *symbol);

  /* VAR_CONFIG instance_specific_init_list END_VAR */
  /*
  SYM_REF2(instance_specific_initializations_c, instance_specific_init_list, unused)
  */

  /* helper symbol for instance_specific_initializations */
  /*
  SYM_LIST(instance_specific_init_list_c)
  */

  /* resource_name '.' program_name '.' {fb_name '.'}
      ((variable_name [location] ':' located_var_spec_init) | (fb_name ':' fb_initialization))
  */
  /*
  SYM_REF6(instance_specific_init_c, resource_name, program_name, any_fb_name_list, variable_name, location, initialization)
  */

  /* helper symbol for instance_specific_init */
  /* function_block_type_name ':=' structure_initialization */
  /*
  SYM_REF2(fb_initialization_c, function_block_type_name, structure_initialization)
  */


  /****************************************/
  /* B.2 - Language IL (Instruction List) */
  /****************************************/
  /***********************************/
  /* B 2.1 Instructions and Operands */
  /***********************************/

  /* | function_name [il_operand_list] */
  // SYM_REF2(il_function_call_c, function_name, il_operand_list)
    void *visit(il_function_call_c *symbol);


  /* | function_name '(' eol_list [il_param_list] ')' */
  // SYM_REF2(il_formal_funct_call_c, function_name, il_param_list)
    void *visit(il_formal_funct_call_c *symbol);


  /*   il_call_operator prev_declared_fb_name
   * | il_call_operator prev_declared_fb_name '(' ')'
   * | il_call_operator prev_declared_fb_name '(' eol_list ')'
   * | il_call_operator prev_declared_fb_name '(' il_operand_list ')'
   * | il_call_operator prev_declared_fb_name '(' eol_list il_param_list ')'
   */
  // SYM_REF4(il_fb_call_c, il_call_operator, fb_name, il_operand_list, il_param_list)
    void *visit(il_fb_call_c *symbol);



  /* | il_operand_list ',' il_operand */
  // SYM_LIST(il_operand_list_c)
    void *visit(il_operand_list_c *symbol);


  /* | il_initial_param_list il_param_instruction */
  // SYM_LIST(il_param_list_c)
    void *visit(il_param_list_c *symbol);

  /*  il_assign_operator il_operand
   * | il_assign_operator '(' eol_list simple_instr_list ')'
   */
  // SYM_REF4(il_param_assignment_c, il_assign_operator, il_operand, simple_instr_list, unused)
    void *visit(il_param_assignment_c *symbol);

  /*  il_assign_out_operator variable */
  // SYM_REF2(il_param_out_assignment_c, il_assign_out_operator, variable);
    void *visit(il_param_out_assignment_c *symbol);


  /*******************/
  /* B 2.2 Operators */
  /*******************/
  /*| [NOT] any_identifier SENDTO */
  // SYM_REF2(il_assign_out_operator_c, option, variable_name)
    void *visit(il_assign_out_operator_c *symbol);




  /***************************************/
  /* B.3 - Language ST (Structured Text) */
  /***************************************/
  /***********************/
  /* B 3.1 - Expressions */
  /***********************/

  /*
  SYM_REF2(function_invocation_c, function_name, parameter_assignment_list)
  */
    void *visit(function_invocation_c *symbol);


  /********************/
  /* B 3.2 Statements */
  /********************/

  /*********************************/
  /* B 3.2.1 Assignment Statements */
  /*********************************/
  /*
  SYM_REF2(assignment_statement_c, l_exp, r_exp)
  */

  /*****************************************/
  /* B 3.2.2 Subprogram Control Statements */
  /*****************************************/
  /*  RETURN */
  // SYM_REF0(return_statement_c)


  /* fb_name '(' [param_assignment_list] ')' */
  /* param_assignment_list -> may be NULL ! */
  // SYM_REF2(fb_invocation_c, fb_name, param_assignment_list)
    void *visit(fb_invocation_c *symbol);

  /* helper symbol for fb_invocation */
  /* param_assignment_list ',' param_assignment */
  // SYM_LIST(param_assignment_list_c)
    void *visit(param_assignment_list_c *symbol);

  /*  variable_name ASSIGN expression */
  // SYM_REF2(input_variable_param_assignment_c, variable_name, expression)
    void *visit(input_variable_param_assignment_c *symbol);

  /* [NOT] variable_name '=>' variable */
  // SYM_REF4(output_variable_param_assignment_c, not_param, variable_name, variable, unused)
    void *visit(output_variable_param_assignment_c *symbol);

  /* helper CLASS for output_variable_param_assignment */
  // SYM_REF0(not_paramassign_c)
  // TODO... ???

};  // function_call_param_iterator_c




