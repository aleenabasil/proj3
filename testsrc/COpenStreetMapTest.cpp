#include "OpenStreetMap.h"
#include "CXMLReader.h"
#include <iostream>
#include <cassert>
#include <sstream>

// Mock XML data for testing purposes
// This simulates a small OpenStreetMap (OSM) dataset with:
// - A single node (ID: 1) located at latitude 12.34, longitude 56.78, labeled "TestNode"
// - A single way (ID: 101) referencing that node and marked as a residential road
const std::string xmlData = R"(
<osm>
    <node id="1" lat="12.34" lon="56.78">
        <tag k="name" v="TestNode"/>
    </node>
    <way id="101">
        <nd ref="1"/>
        <tag k="highway" v="residential"/>
    </way>
</osm>
)";

// Test case: Ensures that nodes are parsed correctly
void TestNodeParsing() {
    // Load the XML into an input stream for processing
    std::istringstream xmlStream(xmlData);
    auto xmlReader = std::make_shared<CXMLReader>(xmlStream);
    
    // Create an OpenStreetMap instance and parse the data
    COpenStreetMap osm(xmlReader);
    
    // Verify that exactly one node was detected
    assert(osm.NodeCount() == 1);

    // Fetch the node and check its properties
    auto node = osm.NodeByIndex(0);
    assert(node);  // Ensure node retrieval was successful
    assert(node->ID() == 1);
    assert(node->Location().first == 12.34);
    assert(node->Location().second == 56.78);

    // Check if the node has an attribute "name" and validate its value
    assert(node->HasAttribute("name"));
    assert(node->GetAttribute("name") == "TestNode");

    std::cout << "TestNodeParsing passed!\n";
}

// Test case: Ensures that ways are parsed correctly
void TestWayParsing() {
    // Load the XML data into an input stream
    std::istringstream xmlStream(xmlData);
    auto xmlReader = std::make_shared<CXMLReader>(xmlStream);
    
    // Create an OpenStreetMap instance and parse the data
    COpenStreetMap osm(xmlReader);
    
    // Verify that exactly one way was detected
    assert(osm.WayCount() == 1);

    // Fetch the way and check its properties
    auto way = osm.WayByIndex(0);
    assert(way);  // Ensure way retrieval was successful
    assert(way->ID() == 101);

    // Ensure the way correctly references one node (ID: 1)
    assert(way->NodeCount() == 1);
    assert(way->GetNodeID(0) == 1);

    // Check if the way has an attribute "highway" and validate its value
    assert(way->HasAttribute("highway"));
    assert(way->GetAttribute("highway") == "residential");

    std::cout << "TestWayParsing passed!\n";
}

// Test case: Ensures that invalid lookups return null pointers instead of crashing
void TestInvalidLookups() {
    // Load the XML data into an input stream
    std::istringstream xmlStream(xmlData);
    auto xmlReader = std::make_shared<CXMLReader>(xmlStream);
    
    // Create an OpenStreetMap instance and parse the data
    COpenStreetMap osm(xmlReader);
    
    // Try to retrieve nodes and ways that do not exist
    assert(osm.NodeByID(999) == nullptr);  // Node ID 999 doesn't exist
    assert(osm.WayByID(999) == nullptr);   // Way ID 999 doesn't exist
    assert(osm.NodeByIndex(10) == nullptr); // There is no 10th node
    assert(osm.WayByIndex(10) == nullptr);  // There is no 10th way

    std::cout << "TestInvalidLookups passed!\n";
}

