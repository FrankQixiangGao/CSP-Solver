def variable_condition_is_valid(val1,  operator, val2):
    if operator == "=":
        return val1 == val2

    if operator == "!":
        return val1 != val2

    if operator == ">":
        return val1 > val2

    if operator == "<":
        return val1 < val2

