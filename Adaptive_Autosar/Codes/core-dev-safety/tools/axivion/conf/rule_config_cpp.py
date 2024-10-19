# This file can be used to customize the configuration if the declarative JSON
# configuration is not flexible enough.
import axivion.config

analysis = axivion.config.get_analysis()

# These options are not yet supported in JSON:
analysis['Generic-LocalInclude'].msg['add_include'].disabled = True
analysis['Generic-LocalInclude'].msg['add_symbol_declaration'].disabled = True
analysis['Generic-LocalInclude'].msg['bad_pch_use'].disabled = True
analysis['Generic-LocalInclude'].msg['circular_include'].disabled = False
analysis['Generic-LocalInclude'].msg['contents_covered_include'].disabled = True
analysis['Generic-LocalInclude'].msg['covered_include'].disabled = True
analysis['Generic-LocalInclude'].msg['local_decl_instead_of_include'].disabled = True
analysis['Generic-LocalInclude'].msg['local_def_instead_of_include'].disabled = True
analysis['Generic-LocalInclude'].msg['more_precise_include'].disabled = True
analysis['Generic-LocalInclude'].msg['move_include'].disabled = True
analysis['Generic-LocalInclude'].msg['move_include_with_many_clients'].disabled = True
analysis['Generic-LocalInclude'].msg['unused_include'].disabled = False

analysis['Generic-ForbiddenFunctions'].blacklist = {
    # file : list of functions
    'stdlib.h': ['malloc', 'free', 'calloc', 'realloc'],
    'stdio.h': ['*printf*'],
    'string.h': ['str[!n]*'],
    ## check for not using boost?
}
