// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//
#include "openvino/c/ov_compiled_model.h"

#include "common.h"

ov_status_e ov_compiled_model_get_runtime_model(const ov_compiled_model_t* compiled_model, ov_model_t** model) {
    if (!compiled_model || !model) {
        return ov_status_e::INVALID_C_PARAM;
    }

    try {
        std::unique_ptr<ov_model_t> _model(new ov_model_t);
        auto runtime_model = compiled_model->object->get_runtime_model();
        _model->object = std::const_pointer_cast<ov::Model>(std::move(runtime_model));
        *model = _model.release();
    }
    CATCH_OV_EXCEPTIONS

    return ov_status_e::OK;
}

ov_status_e ov_compiled_model_inputs(const ov_compiled_model_t* compiled_model, ov_output_node_list_t* input_nodes) {
    if (!compiled_model || !input_nodes) {
        return ov_status_e::INVALID_C_PARAM;
    }

    try {
        auto inputs = compiled_model->object->inputs();
        int num = inputs.size();
        input_nodes->size = num;
        std::unique_ptr<ov_output_const_node_t[]> _output_nodes(new ov_output_const_node_t[num]);
        for (int i = 0; i < num; i++) {
            _output_nodes[i].object = std::make_shared<ov::Output<const ov::Node>>(std::move(inputs[i]));
        }
        input_nodes->output_nodes = _output_nodes.release();
    }
    CATCH_OV_EXCEPTIONS

    return ov_status_e::OK;
}

ov_status_e ov_compiled_model_outputs(const ov_compiled_model_t* compiled_model, ov_output_node_list_t* output_nodes) {
    if (!compiled_model || !output_nodes) {
        return ov_status_e::INVALID_C_PARAM;
    }

    try {
        auto outputs = compiled_model->object->outputs();
        int num = outputs.size();
        output_nodes->size = num;
        std::unique_ptr<ov_output_const_node_t[]> _output_nodes(new ov_output_const_node_t[num]);
        for (int i = 0; i < num; i++) {
            _output_nodes[i].object = std::make_shared<ov::Output<const ov::Node>>(std::move(outputs[i]));
        }
        output_nodes->output_nodes = _output_nodes.release();
    }
    CATCH_OV_EXCEPTIONS

    return ov_status_e::OK;
}

ov_status_e ov_compiled_model_create_infer_request(const ov_compiled_model_t* compiled_model,
                                                   ov_infer_request_t** infer_request) {
    if (!compiled_model || !infer_request) {
        return ov_status_e::INVALID_C_PARAM;
    }

    try {
        std::unique_ptr<ov_infer_request_t> _infer_request(new ov_infer_request_t);
        auto infer_req = compiled_model->object->create_infer_request();
        _infer_request->object = std::make_shared<ov::InferRequest>(std::move(infer_req));
        *infer_request = _infer_request.release();
    }
    CATCH_OV_EXCEPTIONS

    return ov_status_e::OK;
}

ov_status_e ov_compiled_model_set_property(const ov_compiled_model_t* compiled_model, const ov_property_t* property) {
    if (!compiled_model || !property) {
        return ov_status_e::INVALID_C_PARAM;
    }

    try {
        compiled_model->object->set_property(property->object);
    }
    CATCH_OV_EXCEPTIONS

    return ov_status_e::OK;
}

ov_status_e ov_compiled_model_get_property(const ov_compiled_model_t* compiled_model,
                                           const ov_property_key_e key,
                                           ov_property_value_t* value) {
    if (!compiled_model || !value) {
        return ov_status_e::INVALID_C_PARAM;
    }

    try {
        switch (key) {
        case ov_property_key_e::SUPPORTED_PROPERTIES: {
            auto supported_properties = compiled_model->object->get_property(ov::supported_properties);
            std::string tmp_s;
            for (const auto& i : supported_properties) {
                tmp_s = tmp_s + "\n" + i;
            }
            char* temp = new char[tmp_s.length() + 1];
            std::copy_n(tmp_s.c_str(), tmp_s.length() + 1, temp);
            value->ptr = static_cast<void*>(temp);
            value->cnt = tmp_s.length() + 1;
            value->type = ov_property_value_type_e::CHAR;
            break;
        }
        default:
            break;
        }
    }
    CATCH_OV_EXCEPTIONS

    return ov_status_e::OK;
}

ov_status_e ov_compiled_model_export_model(const ov_compiled_model_t* compiled_model, const char* export_model_path) {
    if (!compiled_model || !export_model_path) {
        return ov_status_e::INVALID_C_PARAM;
    }
    try {
        std::ofstream model_file(export_model_path, std::ios::out | std::ios::binary);
        if (model_file.is_open()) {
            compiled_model->object->export_model(model_file);
        } else {
            return ov_status_e::GENERAL_ERROR;
        }
    }
    CATCH_OV_EXCEPTIONS
    return ov_status_e::OK;
}

void ov_compiled_model_free(ov_compiled_model_t* compiled_model) {
    if (compiled_model)
        delete compiled_model;
}