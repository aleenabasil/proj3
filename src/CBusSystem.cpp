// Bus System Stop member functions
// Returns the stop id of the stop
TStopID ID() const noexcept override;

// Returns the node id of the bus stop
CStreetMap::TNodeID NodeID() const noexcept override;


// Bus System Route member functions
// Returns the name of the route
std::string Name() const noexcept override;


// Returns the number of stops on the route
std::size_t StopCount() const noexcept override;

// Returns the stop id specified by the index, returns InvalidStopID if index
// is greater than or equal to StopCount()
TStopID GetStopID(std::size_t index) const noexcept override;