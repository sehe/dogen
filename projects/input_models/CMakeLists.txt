# -*- mode: cmake; tab-width: 4; indent-tabs-mode: nil -*-
#
# Copyright (C) 2012 Kitanda <info@kitanda.co.uk>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
set(projects_dir "${CMAKE_SOURCE_DIR}/projects")
set(models_dir "${projects_dir}/input_models")

set(target yarn)
add_custom_target(knit_${target}
    COMMENT "Knitting Yarn model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --reference ${models_dir}/annotations.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/test/.*
    --ignore-files-matching-regex .*/tests/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target yarn.json)
add_custom_target(knit_${target}
    COMMENT "Knitting Yarn JSON model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --reference ${models_dir}/annotations.dia,dogen
    --reference ${models_dir}/yarn.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*/test/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target dia)
add_custom_target(knit_${target}
    COMMENT "Knitting Dia model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/test/.*
    --ignore-files-matching-regex .*/tests/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target options)
add_custom_target(knit_${target}
    COMMENT "Knitting Options model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/test/.*
    --ignore-files-matching-regex .*/tests/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target quilt.cpp)
add_custom_target(knit_${target}
    COMMENT "Knitting Quilt C++ model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --reference ${models_dir}/options.dia,dogen
    --reference ${models_dir}/yarn.dia,dogen
    --reference ${models_dir}/formatters.dia,dogen
    --reference ${models_dir}/annotations.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*stitch
    --ignore-files-matching-regex .*_stitch.hpp
    --ignore-files-matching-regex .*_stitch.cpp
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target database)
add_custom_target(knit_${target}
    COMMENT "Knitting database model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen::test_models
    --cpp-project-dir ${projects_dir}/test_models
    --ignore-files-matching-regex .*CMakeLists.txt
    --ignore-files-matching-regex .*/test/.*
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*sql
    --ignore-files-matching-regex .*-odb.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target knit)
add_custom_target(knit_${target}
    COMMENT "Knitting knit model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --reference ${models_dir}/annotations.dia,dogen
    --reference ${models_dir}/options.dia,dogen
    --reference ${models_dir}/yarn.dia,dogen
    --reference ${models_dir}/quilt.cpp.dia,dogen
    --reference ${models_dir}/formatters.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target yarn.dia)
add_custom_target(knit_${target}
    COMMENT "Knitting Yarn Dia model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --reference ${models_dir}/dia.dia,dogen
    --reference ${models_dir}/annotations.dia,dogen
    --reference ${models_dir}/yarn.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*/test/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target formatters)
add_custom_target(knit_${target}
    COMMENT "Knitting the Formatters Model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*/test/.*
    --verbose
    --delete-extra-files)

if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target quilt)
add_custom_target(knit_${target}
    COMMENT "Knitting Quilt model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --reference ${models_dir}/options.dia,dogen
    --reference ${models_dir}/annotations.dia,dogen
    --reference ${models_dir}/yarn.dia,dogen
    --reference ${models_dir}/formatters.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*/test/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target annotations)
add_custom_target(knit_${target}
    COMMENT "Knitting Annotations model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*/test/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})

set(target stitch)
add_custom_target(knit_${target}
    COMMENT "Knitting Stitch model" VERBATIM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${knitter_binary}
    --target ${models_dir}/${target}.dia,dogen
    --reference ${models_dir}/annotations.dia,dogen
    --reference ${models_dir}/formatters.dia,dogen
    --cpp-project-dir ${projects_dir}/
    --ignore-files-matching-regex .*/CMakeLists.txt
    --ignore-files-matching-regex .*/tests/.*
    --ignore-files-matching-regex .*/test/.*
    --verbose
    --delete-extra-files)
if(use_latest_knitter)
    add_dependencies(knit_${target} knitter)
endif()
add_dependencies(knit_all knit_${target})