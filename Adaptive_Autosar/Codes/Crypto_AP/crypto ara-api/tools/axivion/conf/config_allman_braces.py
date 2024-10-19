import axivion.config

analysis = axivion.config.get_analysis()


### Configuration of AllmanBraces

analysis['CodingStyle-AllmanBraces'].expected_indent = {
    'Switch_Case_Label': 0,
    'Catch_Block': 0,  # relative to try
    'Named_Label': '1',  # means: fixed column 1
    'Undef_Directive': '1',
    'Include_Interface': '1',
    'Line_Directive': '1',
    'Conditional_Compilation_Interface': '1',
    'Macro_Definition': '1',
    'Statement': 2,  # the default indentation
}
