// Define the implementation structure for COpenStreetMap
struct COpenStreetMap::SImplementation {
    // Forward declare implementation classes
    class SNodeImpl;
    class SWayImpl;
    
    // Vectors to store nodes and ways
    std::vector<std::shared_ptr<SNodeImpl>> Nodes;
    std::vector<std::shared_ptr<SWayImpl>> Ways;
};

// Define the node implementation class inheriting from CStreetMap::SNode
class COpenStreetMap::SImplementation::SNodeImpl : public CStreetMap::SNode {
public:
    TNodeID NodeID;  // Node ID
    TLocation NodeLocation;  // Coordinates (latitude, longitude)
    std::unordered_map<std::string, std::string> Attributes;  // Key-value attributes

    // Get the node's ID
    TNodeID ID() const noexcept override {
        return NodeID;
    }

    // Get the node's location
    TLocation Location() const noexcept override {
        return NodeLocation;
    }

    // Get the number of attributes the node has
    std::size_t AttributeCount() const noexcept override {
        return Attributes.size();
    }

    // Retrieve an attribute key by index
    std::string GetAttributeKey(std::size_t index) const noexcept override {
        if (index < Attributes.size()) {
            auto it = Attributes.begin();
            std::advance(it, index);
            return it->first;
        }
        return ""; // Return empty string if index is out of bounds
    }

    // Check if the node has a specific attribute
    bool HasAttribute(const std::string &key) const noexcept override {
        return Attributes.find(key) != Attributes.end();
    }

    // Retrieve an attribute value by key
    std::string GetAttribute(const std::string &key) const noexcept override {
        auto it = Attributes.find(key);
        return (it != Attributes.end()) ? it->second : "";
    }
};

// Define the way implementation class inheriting from CStreetMap::SWay
class COpenStreetMap::SImplementation::SWayImpl : public CStreetMap::SWay {
public:
    TWayID WayID;  // Way ID
    std::vector<TNodeID> NodeIDs;  // List of node IDs forming the way
    std::unordered_map<std::string, std::string> Attributes;  // Key-value attributes

    // Get the way's ID
    TWayID ID() const noexcept override {
        return WayID;
    }

    // Get the number of nodes in the way
    std::size_t NodeCount() const noexcept override {
        return NodeIDs.size();
    }

    // Retrieve a node ID by index
    TNodeID GetNodeID(std::size_t index) const noexcept override {
        return (index < NodeIDs.size()) ? NodeIDs[index] : CStreetMap::InvalidNodeID;
    }

    // Get the number of attributes
    std::size_t AttributeCount() const noexcept override {
        return Attributes.size();
    }

    // Retrieve an attribute key by index
    std::string GetAttributeKey(std::size_t index) const noexcept override {
        if (index < Attributes.size()) {
            auto it = Attributes.begin();
            std::advance(it, index);
            return it->first;
        }
        return "";
    }

    // Check if the way has a specific attribute
    bool HasAttribute(const std::string &key) const noexcept override {
        return Attributes.find(key) != Attributes.end();
    }

    // Retrieve an attribute value by key
    std::string GetAttribute(const std::string &key) const noexcept override {
        auto it = Attributes.find(key);
        return (it != Attributes.end()) ? it->second : "";
    }
};

// Constructor: Initializes the implementation and parses the XML file
COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src) {
    DImplementation = std::make_unique<SImplementation>();
    SXMLEntity entity;
    std::shared_ptr<SImplementation::SNodeImpl> currentNode = nullptr;
    std::shared_ptr<SImplementation::SWayImpl> currentWay = nullptr;

    // Parse the XML file
    while (src->ReadEntity(entity)) {
        if (entity.DType == SXMLEntity::EType::StartElement) {
            if (entity.DNameData == "node") {
                currentNode = std::make_shared<SImplementation::SNodeImpl>();
                currentWay = nullptr;

                // Process node attributes
                for (const auto& attr : entity.DAttributes) {
                    if (attr.first == "id") {
                        currentNode->NodeID = std::stoull(attr.second);
                    } else if (attr.first == "lat") {
                        currentNode->NodeLocation.first = std::stod(attr.second);
                    } else if (attr.first == "lon") {
                        currentNode->NodeLocation.second = std::stod(attr.second);
                    } else {
                        currentNode->Attributes[attr.first] = attr.second;
                    }
                }
            } else if (entity.DNameData == "way") {
                currentWay = std::make_shared<SImplementation::SWayImpl>();
                currentNode = nullptr;

                for (const auto& attr : entity.DAttributes) {
                    if (attr.first == "id") {
                        currentWay->WayID = std::stoull(attr.second);
                    } else {
                        currentWay->Attributes[attr.first] = attr.second;
                    }
                }
            } else if (entity.DNameData == "nd" && currentWay) {
                for (const auto& attr : entity.DAttributes) {
                    if (attr.first == "ref") {
                        currentWay->NodeIDs.push_back(std::stoull(attr.second));
                    }
                }
            } else if (entity.DNameData == "tag") {
                std::string key, value;
                for (const auto& attr : entity.DAttributes) {
                    if (attr.first == "k") key = attr.second;
                    else if (attr.first == "v") value = attr.second;
                }
                if (!key.empty()) {
                    if (currentNode) currentNode->Attributes[key] = value;
                    else if (currentWay) currentWay->Attributes[key] = value;
                }
            }
        } else if (entity.DType == SXMLEntity::EType::EndElement) {
            if (entity.DNameData == "node" && currentNode) {
                DImplementation->Nodes.push_back(currentNode);
                currentNode = nullptr;
            } else if (entity.DNameData == "way" && currentWay) {
                DImplementation->Ways.push_back(currentWay);
                currentWay = nullptr;
            }
        }
    }
}

// Destructor
COpenStreetMap::~COpenStreetMap() = default;

// Retrieve counts and elements
std::size_t COpenStreetMap::NodeCount() const noexcept { return DImplementation->Nodes.size(); }
std::size_t COpenStreetMap::WayCount() const noexcept { return DImplementation->Ways.size(); }
std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept { return (index < DImplementation->Nodes.size()) ? DImplementation->Nodes[index] : nullptr; }
std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept {
    for (auto& node : DImplementation->Nodes) if (node->ID() == id) return node;
    return nullptr;
}
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept { return (index < DImplementation->Ways.size()) ? DImplementation->Ways[index] : nullptr; }
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept {
    for (auto& way : DImplementation->Ways) if (way->ID() == id) return way;
    return nullptr;
}
