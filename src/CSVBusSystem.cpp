



// CCSVBusSystem member functions
// Constructor for the CSV Bus System
CCSVBusSystem(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr<CDSVReader > routesrc);


// Destructor for the CSV Bus System
~CCSVBusSystem();

// Returns the number of stops in the system
std::size_t StopCount() const noexcept override;



// Returns the number of routes in the system
std::size_t RouteCount() const noexcept override;


// Returns the SStop specified by the index, nullptr is returned if index is
// greater than equal to StopCount()
std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept override;



// Returns the SStop specified by the stop id, nullptr is returned if id is
// not in the stops
std::shared_ptr<SStop> StopByID(TStopID id) const noexcept override;


// Returns the SRoute specified by the index, nullptr is returned if index is
// greater than equal to RouteCount()
std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept override;


 // Returns the SRoute specified by the name, nullptr is returned if name is
// not in the routes
std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept override;
