#include "analyzer.h"

void Analyzer_Result::to_json_add_array(Json::Value &root,
                                        std::string name,
                                        std::vector<std::string> array) const
{
    if (array.size() == 0) {
        return;
    }
    Json::Value json_array(Json::arrayValue);

    for (std::vector<std::string>::const_iterator it = array.begin();
         it != array.end(); it++) {
        json_array.append((*it));
    }

    root[name] = json_array;
}

void Analyzer_Result::to_json(Json::Value &root) const
{
    root["status"] = status_as_string();
    root["evilness"] = evilness();
    root["severity-score"] = evilness();

    const std::string *my_reason = reason();
    if (my_reason != NULL) {
        root["reason"] = *my_reason;
    }

    to_json_add_array(root, "series", _series);
    to_json_add_array(root, "evidence", _evidence);
}

void Analyzer_Result_Period::to_json(Json::Value &root) const
{
    Analyzer_Result::to_json(root);
    root["timestamp_start"] = (Json::UInt64)_T_start;
    root["timestamp_stop"] = (Json::UInt64)_T_stop;
    root["duration"] = (_T_stop - _T_start) / 1000000.0f;
    root["duration-units"] = "seconds";
}

void Analyzer_Result_Event::to_json(Json::Value &root) const
{
    Analyzer_Result::to_json(root);
    root["timestamp"] = (Json::UInt64)_T;
}


uint32_t Analyzer::evilness() const {
    uint32_t ret = 0; // remove _evilness here?
    std::vector<Analyzer_Result*> my_results = results();
    for (std::vector<Analyzer_Result*>::const_iterator it = my_results.begin();
         it != my_results.end();
         it++) {
        ret += (*it)->evilness();
    }
    return ret;
}

void Analyzer::results_json_results(Json::Value &root)
{
    std::vector<Analyzer_Result*> my_results = results();
    for (std::vector<Analyzer_Result*>::const_iterator it = my_results.begin();
         it != my_results.end();
         it++) {
        Json::Value result(Json::objectValue);
        (*it)->to_json(result);
        root.append(result);
    }
}
