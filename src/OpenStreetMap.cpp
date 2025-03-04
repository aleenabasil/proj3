#include "OpenStreetMap.h"
#include <memory>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

// struct for COpenStreetMap
struct COpenStreetMap::SImplementation {
    // the inner classes for node and way implementations
    class SNodeImpl;
    class SWayImpl;
    
    // the vectors to hold nodes and different ways
    std::vector<std::shared_ptr<SNodeImpl>> nodeList;
    std::vector<std::shared_ptr<SWayImpl>> wayList;
};

// node class implementation
class COpenStreetMap::SImplementation::SNodeImpl : public CStreetMap::SNode {
public:
    TNodeID nodeID; // id for node
    TLocation nodeLocation; // location (lat, long) for node
    std::unordered_map<std::string, std::string> nodeAttributes; // attributes for node

    // override methods for node
    TNodeID ID() const noexcept override { return nodeID; }
    TLocation Location() const noexcept override { return nodeLocation; }
    std::size_t AttributeCount() const noexcept override { return nodeAttributes.size(); }
    
    // get attribute key by index
    std::string GetAttributeKey(std::size_t index) const noexcept override {
        if (index < nodeAttributes.size()) {
            auto it = nodeAttributes.begin();
            std::advance(it, index);
            return it->first;
        }
        return "";
    }

    // check if the node has a specific attribute
    bool HasAttribute(const std::string &key) const noexcept override { return nodeAttributes.find(key) != nodeAttributes.end(); }
    
    // get attribute value by key
    std::string GetAttribute(const std::string &key) const noexcept override {
        auto it = nodeAttributes.find(key);
        return (it != nodeAttributes.end()) ? it->second : "";
    }
};

// implementation of way class, this inherits from CStreetMap::SWay
class COpenStreetMap::SImplementation::SWayImpl : public CStreetMap::SWay {
public:
    TWayID wayID; // id for way
    std::vector<TNodeID> nodeIDs; // vector to hold node ids for way
    std::unordered_map<std::string, std::string> wayAttributes; // attributes for way

    // override methods for way
    TWayID ID() const noexcept override { return wayID; }
    std::size_t NodeCount() const noexcept override { return nodeIDs.size(); }
    
    // get node id by index in way
    TNodeID GetNodeID(std::size_t index) const noexcept override {
        if (index < nodeIDs.size()) {
            return nodeIDs[index];
        } else {
            return CStreetMap::InvalidNodeID; // return invalid node id if index is out of range
        }
    }

    // get attribute count for way
    std::size_t AttributeCount() const noexcept override { return wayAttributes.size(); }
    
    // get attribute key by index
    std::string GetAttributeKey(std::size_t index) const noexcept override {
        if (index < wayAttributes.size()) {
            auto it = wayAttributes.begin();
            std::advance(it, index);
            return it->first;
        }
        return "";
    }

    // check if the way has a specific attribute
    bool HasAttribute(const std::string &key) const noexcept override { return wayAttributes.find(key) != wayAttributes.end(); }
    
    // get attribute value by key
    std::string GetAttribute(const std::string &key) const noexcept override {
        auto it = wayAttributes.find(key);
        if (it != wayAttributes.end()) {
            return it->second;
        }
        return "";
    }
};

// constructor for COpenStreetMap that takes a shared pointer to CXMLReader
COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> xmlReader) {
    // initialize the implementation
    DImplementation = std::make_unique<SImplementation>();
    
    SXMLEntity xmlEntity; // variable to hold the xml entity
    
    std::shared_ptr<SImplementation::SNodeImpl> currNode = nullptr; // this points to current node
    std::shared_ptr<SImplementation::SWayImpl> currWay = nullptr; // this points to current way

    // read the xml entities
    while (xmlReader->ReadEntity(xmlEntity)) {
        // entity is a start element
        if (xmlEntity.DType == SXMLEntity::EType::StartElement) {
            
            // if the entity is a node
            if (xmlEntity.DNameData == "node") {
                // it will create a new NodeImplemntation object
                currNode = std::make_shared<SImplementation::SNodeImpl>();
                currWay = nullptr; // no way is associated with node

                // parse attributes of node and assign to currNode
                for (const auto& attr : xmlEntity.DAttributes) {
                    if (attr.first == "id") {
                        currNode->nodeID = std::stoull(attr.second);
                    } 
                    // if the key is not id, then assign the value to the key
                    else if (attr.first == "lat") {
                        currNode->nodeLocation.first = std::stod(attr.second);
                    } 
                    // if the key is not id or lat, then assign the value to the key
                    else if (attr.first == "lon") {
                        currNode->nodeLocation.second = std::stod(attr.second);
                    } 
                    // if the key is not id, lat, or lon, then assign the value to the key
                    else {
                        currNode->nodeAttributes[attr.first] = attr.second;
                    }
                }
            } 

            // if the entity is a way, then create a new WayImplementation
            else if (xmlEntity.DNameData == "way") {
                currWay = std::make_shared<SImplementation::SWayImpl>();
                currNode = nullptr; // node is not associated with way

                // parse attributes of way and assign to currWay
                for (const auto& attr : xmlEntity.DAttributes) {
                    if (attr.first == "id") {
                        currWay->wayID = std::stoull(attr.second);
                    } 
                    // if the key is not id, then assign the value to the key
                    else {
                        currWay->wayAttributes[attr.first] = attr.second;
                    }
                }
            } 
            // if the entity is a nd
            else if (xmlEntity.DNameData == "nd" && currWay) {
                
                // parse attributes of nd and assign to currWay
                for (const auto& attr : xmlEntity.DAttributes) {
                    if (attr.first == "ref") {
                        currWay->nodeIDs.push_back(std::stoull(attr.second));
                    }
                }
            } 
            // if the entity is a tag
            else if (xmlEntity.DNameData == "tag") {
                std::string key, value;

                // parse attributes of tag and assign to key and value
                for (const auto& attr : xmlEntity.DAttributes) {
                    if (attr.first == "k") {
                        key = attr.second;
                    } 
                    // if the key is empty, then assign the value to the key
                    else if (attr.first == "v") {
                        value = attr.second;
                    }
                }

                // if the key is not empty, then assign the value to the key
                if (!key.empty()) {
                    if  (currNode) {
                        currNode->nodeAttributes[key] = value;
                    } 
                    // if the entity is a way and currWay is not null, then assign the value to the key
                    else if (currWay) {
                        currWay->wayAttributes[key] = value;
                    }
                }
            }
        } 
        // if the entity is an end element
        else if (xmlEntity.DType == SXMLEntity::EType::EndElement) {
            
            // if the entity is a node and currNode is not null, then add it to the nodeList
            if (xmlEntity.DNameData == "node" && currNode) {
                DImplementation->nodeList.push_back(currNode);
                currNode = nullptr; // reset currNode
            } 
            // if the entity is a way and currWay is not null, then add it to the wayList
            else if (xmlEntity.DNameData == "way" && currWay) {
                DImplementation->wayList.push_back(currWay);
                currWay = nullptr; // reset currWay
            }
        }
    }
}

// destructor for COpenStreetMap
COpenStreetMap::~COpenStreetMap() = default;

// get the number of nodes
std::size_t COpenStreetMap::NodeCount() const noexcept {
    return DImplementation->nodeList.size();
}

// get the number of ways
std::size_t COpenStreetMap::WayCount() const noexcept {
    return DImplementation->wayList.size();
}

// get the node by index
std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept {
    if (index < DImplementation->nodeList.size()) {
        return DImplementation->nodeList[index];
    }
    return nullptr;
}

// get node by id
std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept {
    for (auto& node : DImplementation->nodeList) {
        if (node->ID() == id) {
            return node;
        }
    }
    return nullptr;
}

// get way by index in wayList
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept {
    if (index < DImplementation->wayList.size()) {
        return DImplementation->wayList[index];
    }
    return nullptr;
}

// get way by its id
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept {
    for (auto& way : DImplementation->wayList) {
        if (way->ID() == id) {
            return way;
        }
    }
    return nullptr;
}
