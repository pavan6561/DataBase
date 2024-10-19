from axivion.config import architecture_helpers
import os.path
import pathlib


# configuration directory where this file is located
confdir = pathlib.Path(__file__).resolve().parent

# GXL files to import as set of pathlib.Path objects
_gxl_architecture = pathlib.Path(
    os.getenv('GXL_ARCHITECTURE', str(confdir / 'Architecture.gxl'))
)
_gxl_mapping = pathlib.Path(os.getenv('GXL_MAPPING', str(confdir / 'Mapping.gxl')))

# Configuration of view names:
# created by RFG generation:
BASE_VIEW = 'Declaration Facts'  # source code representation
HIERARCHY_VIEW = 'File'  # hierarchical structure of the BASE_VIEW
# imported from GXL files:
ARCHITECTURE_VIEW = (
    'Gravis Architecture'
)  # technical architecture to check against BASE_VIEW
MAPPING_VIEW = 'Gravis Mapping'  # mapping from HIERARCHY_VIEW to ARCHITECTURE_VIEW


# Add steps in execution order with configuration
steps = architecture_helpers.SequenceHelper(default_prefix='GravisArchitecture')

# Import architecture model (GXL) into the RFG's ARCHITECTURE_VIEW
steps.add(
    'GXLImport',
    new_name='ImportGXLArchitecture',
    options={'gxl_file': _gxl_architecture, 'view_name': ARCHITECTURE_VIEW},
)

# Import mapping of source to architecture model (GXL) into the RFG's MAPPING_VIEW
steps.add(
    'GXLImport',
    new_name='ImportGXLMapping',
    options={'gxl_file': _gxl_mapping, 'view_name': MAPPING_VIEW},
)

# Perform architecture check
steps.add(
    'ArchitectureCheck',
    options={
        'architecture_view_name': ARCHITECTURE_VIEW,
        'mapping_view_name': MAPPING_VIEW,
        'hierarchy_view_name': HIERARCHY_VIEW,
        'base_view_name': BASE_VIEW,
    },
)
