// COpenStreetMap member functions
// Constructor for the Open Street Map
COpenStreetMap(std::shared_ptr<CXMLReader> src);

// Destructor for the Open Street Map
~COpenStreetMap();

// Returns the number of nodes in the map
std::size_t NodeCount() const noexcept override;

// Returns the number of ways in the map
std::size_t WayCount() const noexcept override;

// Returns the SNode associated with index, returns nullptr if index is
// larger than or equal to NodeCount()
std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept override;

// Returns the SNode with the id of id, returns nullptr if doesn't exist
std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept override;

// Returns the SWay associated with index, returns nullptr if index is
// larger than or equal to WayCount()
std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept override;

// Returns the SWay with the id of id, returns nullptr if doesn't exist
std::shared_ptr<SWay> WayByID(TWayID id) const noexcept override;
