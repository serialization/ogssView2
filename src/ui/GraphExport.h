//
// Created by feldentm on 02.08.20.
//

#ifndef OGSS_VIEW_PP_GRAPHEXPORT_H
#define OGSS_VIEW_PP_GRAPHEXPORT_H

#include "../empty/File.h"

/**
 * Export a portion of the graph to dot and allow the user to perform further
 * actions on it. Export always starts at a value and is done using a breadth
 * first search until the supplied limits are reached or all nodes have been
 * visited.
 *
 * The dot presentation uses nodes for containers and objects.
 * Edges are pointers between nodes. The fields are used as edge labels.
 * Node labels are created from non-pointer field data.
 * If a limit is reached, the respective node will receive a "..." in its body.
 *
 * All node presentations are created before starting to write the file.
 */
class GraphExport final {
    const std::string &name;
    const int maxDistance;
    const int maxContainerItems;
    int remainingValues;

    struct Action {
        const std::string &target;
        const ogss::fieldTypes::FieldType *const ft;
        const ogss::api::Box val;
    };

    //! if a node has a name, it wont be followed
    std::unordered_map<ogss::api::Box, std::string> names;

    //! every named node has a body that is at least its name
    std::unordered_map<std::string, std::string> bodies;

    //! edges can be created immediately as they will not be traversed twice
    //! anyway
    std::stringstream edges;

    //! remaining
    std::deque<Action> todo;

    //! process the next entry in todo
    void next();

  public:
    GraphExport(const std::string &name, int maxDistance, int maxContainerItems,
                int maxNodes, const ogss::fieldTypes::FieldType *ft,
                ogss::api::Box root);

    void openXDOT();

    void openPDF();
};

#endif // OGSS_VIEW_PP_GRAPHEXPORT_H
