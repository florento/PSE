//
//  PSOrder.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/02/2023.
//
/// @addtogroup pitch
/// @{ 


#include "PSOrder.hpp"


namespace pse {


bool PSClex::operator()(std::shared_ptr<const PSC0>& lhs,
                        std::shared_ptr<const PSC0>& rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost() == rhs->cost())
        return (lhs->id() < rhs->id());      // largest index
    else
        return (lhs->cost() > rhs->cost());  // smallest cost
//  if (lhs->accidentals() == rhs->accidentals())
//  {
//      // dist, lexicographically
//      if (lhs->dist() == rhs->dist())
//      {
//          // disjoint moves, lexicographically
//          if (lhs->disjoint() == rhs->disjoint())
//              return false; // tie break fail
//          else
//              return (lhs->disjoint() > rhs->disjoint());  // smallest nb mv disjoint moves
//      }
//      else
//          return (lhs->dist() > rhs->dist());  // smallest dist
//  }
//  else
//      return (lhs->accidentals() > rhs->accidentals());  // smallest cost
};


bool PSClexc::operator()(std::shared_ptr<const PSC1c>& lhs,
                         std::shared_ptr<const PSC1c>& rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost() == rhs->cost())
        return (lhs->id() < rhs->id());      // largest index
    else
        return (lhs->cost() > rhs->cost());  // smallest cost
};

//bool PSCcumul::operator()(std::shared_ptr<const PSC0>& lhs,
//                          std::shared_ptr<const PSC0>& rhs)
//{
//    assert (lhs);
//    assert (rhs);
//    if (lhs->cost().eq_cumul(rhs->cost()))
//        return (lhs->id() < rhs->id()); // largest index
//    else
//        return (lhs->cost().greater_cumul(rhs->cost()));
//        // smallest cost
//}


//bool PSCad::operator()(std::shared_ptr<const PSC0>& lhs,
//                       std::shared_ptr<const PSC0>& rhs)
//{
//    assert (lhs);
//    assert (rhs);
//    if (lhs->cost().getAccid() == rhs->cost().getAccid())
//        return (lhs->id() < rhs->id());  // largest index
//    else
//        return (lhs->cost().getAccid() > rhs->cost().getAccid());
//};


//bool PSCaplusd::operator()(std::shared_ptr<const PSC0>& lhs,
//                           std::shared_ptr<const PSC0>& rhs)
//{
//    assert (lhs);
//    assert (rhs);
//    if (lhs->cost().getAccid() + lhs->cost().getDia() ==
//        rhs->cost().getAccid() + rhs->cost().getDia())
//    {
//        if (lhs->cost().getDist() == rhs->cost().getDist())
//        {
//            if (lhs->cost().getColor() == rhs->cost().getColor())
//                return (lhs->id() < rhs->id()); // largest index
//            else
//                return (lhs->cost().getColor() > rhs->cost().getColor());
//       }
//        else
//            return (lhs->cost().getDist() > rhs->cost().getDist());
//    }
//    else
//        return (lhs->cost().getAccid() + lhs->cost().getDia() >
//                rhs->cost().getAccid() + rhs->cost().getDia());
//};


//bool PSCdist::operator()(std::shared_ptr<const PSC0>& lhs,
//                         std::shared_ptr<const PSC0>& rhs)
//{
//    assert (lhs);
//    assert (rhs);
//    return (lhs->cost().getDist() > rhs->cost().getDist());
//};


std::ostream& operator<<(std::ostream& o, const CostOrdering& co)
{
    switch (co)
    {
        case CostOrdering::Default:
            o << "Ord_Default";
            break;

        case CostOrdering::Lex:
            o << "Ord_Lex";
            break;

        case CostOrdering::Cumul:
            o << "Ord_Cumul";
            break;

        case CostOrdering::Approx:
            o << "Ord_Approx";
            break;

        case CostOrdering::Undef:
            o << "Ord_Undef";
            break;

        default:
        {
            ERROR("unknown Ton mode");
            o << "Ord_ERR";
            break;
        }
    }
    return o;
}


} // end namespace pse

/// @}

