import axivion.config

analysis = axivion.config.get_analysis()


### Filtering out certain CompilerWarnings


def filter_compiler_warnings(node):
    return node.Static_Type not in (
        1388,  # cafe_base_class_has_different_dll_interface,
        1309,  # cafe_dll_interface_conflict_dllexport_assumed,
        1394,  # cafe_field_without_dll_interface,
    )


analysis['Generic-NoCompilerWarnings'].message_predicate = filter_compiler_warnings
