// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0
#ifndef __IPCVERSIONS__
#define __IPCVERSIONS__

#define IPC_VERSION_MAJOR 1
#define IPC_VERSION_MINOR 0
#define IPC_VERSION_PATCH 0
#define IPC_VERSION_TWEAK 0

#define IPC_LATEST_RELEASE_VERSION "@PROJECT_VERSION@@IPC_VERSION_SUFFIX@"
#define IPC_BUILDDATE "@IPC_BUILDDATE@"
#define IPC_SHA1 "@IPC_SHA1@"

#define IPC_PRINT_BUILDINFO() std::cout << "Built: " << IPC_BUILDDATE << std::endl;

#endif
