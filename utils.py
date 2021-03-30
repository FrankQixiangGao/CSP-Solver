
import re

# util function to get the domains and conditions from the command line arguments provided
def get_domains_and_conditions(argv):
    # check the number of command line arguments provided
    if len(argv) != 4:
        print(f'ERROR: Provided {str(len(argv))} command line argument when expected 3')
        exit()

    # regex patterns and variables to extract data from file lines
    var_pattern = "^([A-Z]): (.*)$"
    con_pattern = "^([A-Z]) (=|!|>|<) ([A-Z])$"

    # dictionary objects to hold domains and conditions
    variable_domains = {}
    variable_conditions = {}

    # read data from var file
    with open(str(argv[1])) as var_file:
        for line in var_file:
            var_match = re.match(var_pattern, line)
            var_name = var_match.group(1)
            var_domain = var_match.group(2).split()
            variable_domains[var_name] = var_domain
            variable_conditions[var_name] = {}

    # read data from con file
    with open(str(argv[2])) as con_file:
        for line in con_file:
            con_match = re.match(con_pattern, line)
            con_var_one = con_match.group(1)
            con_op = con_match.group(2)
            con_var_two = con_match.group(3)

            variable_conditions[con_var_one][con_var_two] = con_op

            if con_op == "=" or con_op == "!":
                variable_conditions[con_var_two][con_var_one] = con_op
            if con_op == ">":
                variable_conditions[con_var_two][con_var_one] = "<"
            if con_op == "<":
                variable_conditions[con_var_two][con_var_one] = ">"

    # read consitency enforcing procedure
    consistency_enforcing_procedure = str(argv[3])
    if consistency_enforcing_procedure != "none" and consistency_enforcing_procedure != "fc":
        consistency_enforcing_procedure = "none"

    return (variable_domains, variable_conditions, consistency_enforcing_procedure)
