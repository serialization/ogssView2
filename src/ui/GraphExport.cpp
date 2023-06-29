//
// Created by feldentm on 02.08.20.
//

#include "GraphExport.h"
GraphExport::GraphExport(const std::string &name, int maxDistance,
                         int maxContainerItems, int maxNodes,
                         const ogss::fieldTypes::FieldType *ft,
                         const ogss::api::Box root) :
  name(name),
  maxDistance(maxDistance),
  maxContainerItems(maxContainerItems),
  remainingValues(maxNodes) {

//    das ist algorithmisch völlig falsch; wir müssen hier kanten in die todo liste eintragen und die knoten separat behandeln
//      dann wird den kanten gefolgt, bis man alle knoten hat und dann werden nurnoch kanten eingetragen, die zwischen vorhandenen knoten liegen

    todo.emplace_back(Action{"root", ft, root});

    while (remainingValues && !todo.empty()) {
        next();
    }

    // calculate all nodes with dots in their body
    {
        std::unordered_set<std::string> hasDots;
        for (auto a : todo) {
            hasDots.insert(a.target);
        }

        // add dot to bodies
        for (auto &n : hasDots) {
            bodies[n] += "\\n...";
        }
    }

//    write???
}
void GraphExport::next() {
    const auto a = todo.front();
    todo.pop_front();

//    ???
}
void GraphExport::openXDOT() {}
void GraphExport::openPDF() {}
