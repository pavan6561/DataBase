
def is_allowed_by_logger(node):
    from bauhaus import ir
    from bauhaus.ir.common.names import named_nodes
    from bauhaus.ir.common.types import pointer_types


    allowed_logging_contexts = ['ara::log::LogStream']
    operator = node.enclosing_of_type('Operator_Notation_Call')

    if (
        operator.is_of_type('Operator_Notation_Call')
        and operator.Call_Target.is_of_type('Operator_Interface')
        and operator.Call_Target.Kind == ir.operator_shift_left
    ):
        if operator.Call_Target.is_of_type('Method'):
            # cases like `logger << "Some String"` where `logger` is of
            # a class that defines operator<<().
            logger = operator.Call_Target.Parent
            if named_nodes.qualified_name(logger) in allowed_logging_contexts:
                return True
        if (
            operator.Call_Target.is_of_type('Operator_Function')
            and operator.Call_Target.Parameters
        ):
            # cases like `std::cout << "Some String"`
            first_param = operator.Call_Target.Parameters[0]
            logger = pointer_types.pointed_to_type(
                first_param.Its_Type.skip_typedefs(True)
            )
            if named_nodes.qualified_name(logger) in allowed_logging_contexts:
                return True
    return False

import axivion.config

analysis = axivion.config.get_analysis()

analysis['AutosarC++19_03-A5.1.1'].allowed_string_contexts.add(is_allowed_by_logger)
