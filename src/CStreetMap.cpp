


// Street Map Node member functions
// Returns the id of the SNode
TNodeID ID() const noexcept override;


// Returns the lat/lon location of the SNode
TLocation Location() const noexcept override;


// Returns the number of attributes attached to the SNode
std::size_t AttributeCount() const noexcept override;


// Returns the key of the attribute at index, returns empty string if index
// is greater than or equal to AttributeCount()
std::string GetAttributeKey(std::size_t index) const noexcept override;


// Returns if the attribute is attached to the SNode
bool HasAttribute(const std::string &key) const noexcept override;


// Returns the value of the attribute specified by key, returns empty string
// if key hasn't been attached to SNode
std::string GetAttribute(const std::string &key) const noexcept override;



// Street Map Way
// Returns the id of the SWay
TWayID ID() const noexcept override;



// Returns the number of nodes in the way
std::size_t NodeCount() const noexcept override;


// Returns the node id of the node at index, returns InvalidNodeID if index
// is greater than or equal to NodeCount()
TNodeID GetNodeID(std::size_t index) const noexcept override;


// Returns the number of attributes attached to the SWay
std::size_t AttributeCount() const noexcept override;


// Returns the key of the attribute at index, returns empty string if index
// is greater than or equal to AttributeCount()
std::string GetAttributeKey(std::size_t index) const noexcept override;


// Returns if the attribute is attached to the SWay
bool HasAttribute(const std::string &key) const noexcept override;


// Returns the value of the attribute specified by key, returns empty string
// if key hasn't been attached to SWay
std::string GetAttribute(const std::string &key) const noexcept override;