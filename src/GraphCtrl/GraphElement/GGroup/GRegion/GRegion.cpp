#include "GRegion.h"//coding
    
    explicit GRegion::GRegion(){

    };
    GRegion::~GRegion() {};

    GRegion::GRegion(const GRegion& region){

    };
    GRegion& GRegion::operator=(const GRegion& region){};

    CSTATUS GRegion::init() {};
    CSTATUS GRegion::deinit() {};
    CSTATUS GRegion::run() {};
    CSTATUS GRegion::process(bool isMock) {};

    CSTATUS GRegion::addElement(GElementPtr element) {};
    