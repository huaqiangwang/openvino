// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/core/node_output.hpp"

#include <pybind11/stl.h>

#include "dict_attribute_visitor.hpp"
#include "pyopenvino/graph/node_output.hpp"

namespace py = pybind11;

template void regclass_graph_Output<ov::Node>(py::module m, std::string typestring);
template void regclass_graph_Output<const ov::Node>(py::module m, std::string typestring);

template <typename T>
void def_type_dependent_functions(py::class_<ov::Output<T>, std::shared_ptr<ov::Output<T>>>& output) {}

template <>
void def_type_dependent_functions<ov::Node>(
    py::class_<ov::Output<ov::Node>, std::shared_ptr<ov::Output<ov::Node>>>& output) {
    output.def("remove_target_input",
               &ov::Output<ov::Node>::remove_target_input,
               py::arg("target_input"),
               R"(
                Removes a target input from the output referenced by this output handle.

                :param target_input: The target input to remove.
                :type target_input: openvino.runtime.Output
               )");
    output.def("replace",
               &ov::Output<ov::Node>::replace,
               py::arg("replacement"),
               R"(
                Replace all users of this value with replacement.

                :param replacement: The node that is a replacement.
                :type replacement: openvino.runtime.Output
               )");
}
