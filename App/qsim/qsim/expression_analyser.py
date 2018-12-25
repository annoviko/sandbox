from qsim.logging import logging


class expression_analyser:
    def __init__(self, expression, session_context):
        if (expression is None) or (len(expression) == 0):
            logging.warning("Code expression is empty - impossible to analyse it.")

        self.__expression = expression
        self.__context = session_context


    def evaluate(self):
        (variables_names, variables_values) = self.__extract_variables()
        for index_variable in range(len(variables_names)):
            internal_variable_name = "variables_values[" + str(index_variable) + "]"
            self.__expression = self.__expression.replace(variables_names[index_variable], internal_variable_name)


        result = None
        try:
            result = eval(self.__expression)
        except:
            logging.error("Impossible to evaluate expression '%s'.", self.__expression)
        
        return result


    def __extract_variables(self):
        variables_names = []
        variables_values = []
        
        start_variable = self.__expression.find("$(", 0)
        while start_variable >= 0:
            stop_variable = self.__expression.find(")", start_variable + 1)
            if stop_variable < 0:
                logging.error("Impossible to parse expression with variable - incorrect format of variables (expression: '%s').", self.__expression)
                break
            
            # extract variable name
            variable_name = self.__expression[start_variable : stop_variable + 1]
            if variable_name not in self.__context.get_variables():
                logging.error("Impossible to find variable '%s' because of not declaration (expression: '%s').", variable_name, self.__expression)
                break
            
            if variable_name not in variables_names:
                variables_names.append(variable_name)
                variables_values.append(self.__context.get_variable(variable_name))
            
            start_variable = self.__expression.find("$(", stop_variable)
        
        return variables_names, variables_values