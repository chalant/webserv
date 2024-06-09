#ifndef ITEMPROUTER_HPP
#define ITEMPROUTER_HPP

/*
 * ITempRouter.hpp
 * Abstract class for the TempRouter
 */

#include "../request/IRequest.hpp"
#include "../response/IResponse.hpp"

typedef std::pair<int, std::pair<int, int> > Triplet_t;

class ITempRouter
{
public:
    virtual ~ITempRouter() {};

	virtual Triplet_t	execRoute(IRoute *route, IRequest *req, IResponse *res) = 0;
	virtual IRoute		*getRoute(IRequest *req, IResponse *res) = 0;
};

#endif // IROUTER_HPP
// Path: includes/IRouter.hpp
