//
//  utils.hpp
//  pse
//
//  Created by Florent Jacquemard on 02/07/2023.
//
/// @addtogroup pitch
/// @{



#ifndef utils_hpp
#define utils_hpp

#include <iostream>

namespace pse {

namespace util {

/// fill the given rank vector with ranks of the values in the given list.
/// @param v vector of values.
/// @param equal equality predicate on the elements of E, for ties in ranking.
/// @param smaller < relation on the elements of E, for ranking.
/// @param rank container for receiving the ranks. must be empty.
/// rank in fill with the rank of each element of v,
/// in the same order as the element given in v.
template<typename E> void ranks(const std::vector<E>& v,
                                std::function<bool (E, E)> equal,
                                std::function<bool (E, E)> smaller,
                                std::vector<size_t>& rank);

//template<typename E>
//std::vector<size_t>
//getRanks(std::vector<std::pair <size_t, E>>& v,
//         std::function<bool (E, E)> smaller);



} // namespace pse

} // namespace util


// separated definition of template functions
#include "utils.tpp"

#endif /* utils_hpp */

/// @}
