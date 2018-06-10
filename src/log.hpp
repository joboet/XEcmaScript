//
//  log.hpp
//  XJS
//
//  Created by Jonas Böttiger on 10.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#ifndef log_hpp
#define log_hpp

#include <string>

class logtype {
public:
    void operator<<(std::string) const;
    void operator<<(const char*) const;
};

const logtype Xlog;

#endif /* log_hpp */
