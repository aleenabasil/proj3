#include "OpenStreetMap.h"
#include "CXMLReader.h"
#include <iostream>
#include <cassert>
#include <sstream>

// ensures that nodes are parsed correctly
void TestNodeParsing() {
    // load the XML into an input stream for processing
    std::istringstream xmlStream(xmlData);
    auto xmlReader = std::make_shared<CXMLReader>(xmlStream);
    
    // create an OpenStreetMap instance and parse the data
    COpenStreetMap osm(xmlReader);
    
    // verify that exactly one node was detected
    assert(osm.NodeCount() == 1);

    // check its properties
    auto node = osm.NodeByIndex(0);
    assert(node);  // Ensure node retrieval was successful
    assert(node->ID() == 1);
    assert(node->Location().first == 12.34);
    assert(node->Location().second == 56.78);

    // check if the node has an attribute "name" and validate its value
    assert(node->HasAttribute("name"));
    assert(node->GetAttribute("name") == "TestNode");

    std::cout << "TestNodeParsing passed!\n";
}

// ensures that ways are parsed correctly
void TestWayParsing() {
    // load the XML data into an input stream
    std::istringstream xmlStream(xmlData);
    auto xmlReader = std::make_shared<CXMLReader>(xmlStream);
    
    // create an OpenStreetMap instance and parse the data
    COpenStreetMap osm(xmlReader);
    
    // verify that exactly one way was detected
    assert(osm.WayCount() == 1);

    // check its properties
    auto way = osm.WayByIndex(0);
    assert(way);  // Ensure way retrieval was successful
    assert(way->ID() == 101);

    // ensure the way correctly references one node
    assert(way->NodeCount() == 1);
    assert(way->GetNodeID(0) == 1);

    std::cout << "TestWayParsing passed!\n";
}

// ensures that invalid lookups return null pointers instead of crashing
void TestInvalidLookups() {
    // load the XML data into an input stream
    std::istringstream xmlStream(xmlData);
    auto xmlReader = std::make_shared<CXMLReader>(xmlStream);
    
    // create an OpenStreetMap instance and parse the data
    COpenStreetMap osm(xmlReader);
    
    // try to retrieve nodes and ways that do not exist
    assert(osm.NodeByID(999) == nullptr);  // Node ID 999 doesn't exist
    assert(osm.WayByID(999) == nullptr);   // Way ID 999 doesn't exist
    assert(osm.NodeByIndex(10) == nullptr); // There is no 10th node
    assert(osm.WayByIndex(10) == nullptr);  // There is no 10th way

    std::cout << "TestInvalidLookups passed!\n";
}

