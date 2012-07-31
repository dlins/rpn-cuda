/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) GridValuesFactory.cc
 */

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "GridValuesFactory.h"
#include <stdexcept>


/*
 * ---------------------------------------------------------------
 * Definitions:
 */




GridValuesFactory::GridValuesFactory(const Physics * physics) : gridArray_(new map<string, GridValues *>()), physics_(physics) {

}

GridValues * GridValuesFactory::getGrid(const string & gridName) {


    if (gridArray_->count(gridName) == 1) {
        return gridArray_->operator [](gridName);
    } else {

        const Boundary* boundary = &physics_->boundary();

        vector<int> noc = getDefaultGridResolution(gridName);

        GridValues * returnedGrid = new GridValues(boundary, boundary->minimums(), boundary->maximums(), noc);


        gridArray_->operator [](gridName) = returnedGrid;

        cout << "Criando grid values " << gridName << endl;


        return returnedGrid;
    }


}

void GridValuesFactory::setResolution(const string & gridName, vector<int> newResolution) {

    GridValues * grid = getGrid(gridName);

    const Boundary* boundary = &physics_->boundary();

    grid->set_grid(boundary, boundary->minimums(), boundary->maximums(), newResolution);

}

vector<int> GridValuesFactory::getDefaultGridResolution(const string & gridName) {


    std::vector<int> noc(2);

    noc[0] = 30;
    noc[1] = 30;


    if (gridName.compare("hugoniotcurve") == 0) {
        noc[0] = 250;
        noc[1] = 250;
    }

    if (gridName.compare("doublecontactcurve") == 0) {
        noc[0] = 30;
        noc[1] = 30;
    }

    if (gridName.compare("bifurcation") == 0) {
        noc[0] = 128;
        noc[1] = 128;
    }
    return noc;


}






