#include "ColorCurve.h"

// Initialize the color table: ATTENTION IT IS ALSO USEFUL TO PUT THIS +'s AND -'s.
//int ColorTable::color[16][3] = { 255, 255, 255, //  0 = Left transport     COLOR = white      ----
//                                 255, 255, 255, //  1                                         +---
//                                 255,   0,   0, //  2 = Choque 2 LAX.      COLOR = red        -+--
//                                 247, 151,  55, //  3 = SUPER_COMPRESSIVE  COLOR = orange     ++--
//                                 255, 255, 255, //  4                                         --+-
//                                 255, 255, 255, //  5                                         +-+-
//                                 255, 255, 255, //  6                                         -++-
//                                 255, 255, 255, //  7                                         +++-
//                                 255,   0, 255, //  8 = EXPANSIVE 2        COLOR = pink       ---+
//                                 255, 255, 255, //  9                                         +--+
//                                  18, 153,   1, // 10 = TRANSITIONAL       COLOR = green      -+-+
//                                   0,   0, 255, // 11 = Choque 1 LAX.      COLOR = dark blue  ++-+
//                                 255, 255, 255, // 12 = SUPER_EXPANSIVE    COLOR = white      --++
//                                 255, 255, 255, // 13                                         +-++
//                                   0, 255, 255, // 14 = EXPANSIVE 1        COLOR = cyan       -+++
//                                 255, 255, 255, // 15 = Right transport    COLOR = white      ++++
//                                };
// Sign function. Inlined, should be fast.

ColorCurve::ColorCurve(const FluxFunction & fluxFunction,
        const AccumulationFunction & accumulationFunction) :
fluxFunction_((FluxFunction *) fluxFunction.clone()),
accFunction_((AccumulationFunction *) accumulationFunction.clone()) {
    
 
    
    
  

}

ColorCurve::~ColorCurve() {
    delete fluxFunction_;
    delete accFunction_;
}

/**
 ** Given an array of points that form a Hugoniot Locus it will return the same curve augmented  with
 ** the shock speed from the reference point and valid eigenvalues at each point. (There are two ways
 ** to introduce the input, (1) from a ShockCurve constucted by continuation and (2) from a vector of
 ** RealVectors given by a finding request, i.e., with the use of Contour.
 **
 ** Each pair of points comprising a segment is consolidated as a HugoniotPolyLine; containing the
 ** coordinates of such points, shock speeds, valid eigenvalues, and the segment type (see as example
 ** the ColorTable above.)
 **
::::: INPUT :::::::::
 **    original_curve: Vector of RealVectors comprising the Hugoniot Locus.
 **              Uref: Reference point of the curve.
 **
::::: OUTPUT ::::::::
 **  classified_curve: Vector of HugoniotPolyLine comprising (in segments) the coordinates, shock
 **                    speeds, eigenvalues and segment type.
 **   transition_list: When the extremes of a segment have different types, extra segments and points
 **                    are introduced, so each segment has a single type. Extra points are stored.
 **
::::: OBSERVATIONS :::::
 **  The dimension of the Conservation Laws is extracted inside the methods, as well as the number of
 **  equations that the RH condition used (valid number of eigenvalues).  All methods are expected to
 **  be general, however, if dimension and number of equations are not equal, the Newton improvements
 **  are not here.
 **/

int ColorCurve::solve(const double *A, double *bb, int dim, double *x) {
    //    int i, j;
    //    int dim = A.rows();
    int nrhs = 1;
    int lda = dim;
    int ipiv[dim];
    int ldb = dim;
    int info;

    // Create a transposed copy of A to be used by LAPACK's dgesv:
    double B[dim][dim];
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) B[j][i] = A[i * dim + j];
    }

    dgesv_(&dim, &nrhs, &B[0][0], &lda, &ipiv[0], &bb[0], &ldb, &info);

    if (info == 0) {
        for (int i = 0; i < dim; i++) x[i] = bb[i];
    }

    return info;
}

void ColorCurve::Left_Newton_improvement(const RealVector &input, const int type, RealVector &out) {
    int dim = input.size();

    double sigma = ref_eigenvalue[type];
    out.resize(dim);

    // TODO: Improve this epsilon
    double epsilon = 1e-10;
    double epsilon2 = epsilon*epsilon;
    double anorm;
    double delta_U[dim];

    double U[dim];
    for (int i = 0; i < dim; i++) U[i] = input.component(i);

    // If this function ever comes to be vectorialized, these lines below are COMMON
    // to all points (since they deal with the ref point).
    //
    double c[dim];
//    double F_ref[dim], G_ref[dim];
//    fluxFunction_->fill_with_jet(dim, ref_point.components(), 0, F_ref, 0, 0);
//    accFunction_->fill_with_jet(dim, ref_point.components(), 0, G_ref, 0, 0);
    for (int i = 0; i < dim; i++) {
        c[i] = sigma * G_ref.component(i) - F_ref.component(i);
    }

    double deltaNorm = 0.0;
    int    count = 0;

    do {
        double F[dim], JF[dim][dim], G[dim], JG[dim][dim];
        double A[dim][dim];
        double b[dim];

        fluxFunction_->fill_with_jet(dim, U, 1, F, &JF[0][0], 0);
        accFunction_->fill_with_jet(dim, U, 1, G, &JG[0][0], 0);

        for (int i = 0; i < dim; i++) {
            // The minus sign is incorporated within the parentesis
            b[i] = (F[i] - sigma * G[i]) + c[i];

            for (int j = 0; j < dim; j++) {
                A[i][j] = sigma * JG[i][j] - JF[i][j];
            }
        }

        // Solve
        if (dim == 2) {
            double det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
            anorm = 0.0;
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    anorm += A[i][j] * A[i][j];
                }
            }

            // If Newton does not converge, return the original point.
            if (fabs(det) <= (epsilon * anorm)) {
                cout << "ColorCurve::Left_Newton does not converge." << endl;
                cout << "count = " << count << " " << input << endl;
                for (int i = 0; i < dim; i++) out.component(i) = input.component(i);
                return;
            }

            // Protect against zero-division or use LAPACK (will be done so anyway for n >= 3)
            delta_U[0] = (b[0] * A[1][1] - b[1] * A[0][1]) / det;
            delta_U[1] = (A[0][0] * b[1] - A[1][0] * b[0]) / det;
        } else {
            // For dimension larger than 2, it uses LAPACK
            int info = solve(&A[0][0], &b[0], dim, & delta_U[0]);

            if (info != 0) {
                cout << "ColorCurve::Left_Newton does not converge." << endl;
                for (int i = 0; i < dim; i++) out.component(i) = input.component(i);
                return;
            }
        }

        // Prepare next step:
        for (int i = 0; i < dim; i++) U[i] += delta_U[i];

        deltaNorm = 0.0;
        for (int i = 0; i < dim; i++) deltaNorm += (delta_U[i] * delta_U[i]);
        count++;
    } while (deltaNorm > epsilon2 && count < 100);

    cout << "Saindo de LN(" << count << "). Input = " << input << ", U = " << U[0]<<" "<<U[1] << endl;

    // Output
    for (int i = 0; i < dim; i++) out.component(i) = U[i];

    return;
}

void ColorCurve::Right_Newton_improvement(const RealVector &input, const int type, RealVector &out) {
    int dim = input.size();
    out.resize(dim);

    // TODO: For dimension larger than 2, the implementation is missing. The input point is returned
    //       as output. (This method pretends to refine the input.)
    if (dim > 2) {
        cout << "ColorCurve::Right_Newton was not implemented for dimension larger than two." << endl;
        for (int i = 0; i < dim; i++) out.component(i) = input.component(i);
        return;
    }

    // TODO: Improve this epsilon
    double epsilon = 1e-10;
    double epsilon2 = epsilon*epsilon;
    double anorm;
    double delta_U[dim];

    double U[dim];
    for (int i = 0; i < dim; i++) U[i] = input.component(i);

//    // If this function ever comes to be vectorialized, these lines below are COMMON
//    // to all points (since they deal with the ref point).
//    //
//    double F_ref[dim], G_ref[dim];
//    fluxFunction_->fill_with_jet(dim, ref_point.components(), 0, F_ref, 0, 0);
//    accFunction_->fill_with_jet(dim, ref_point.components(), 0, G_ref, 0, 0);

    double deltaNorm = 0.0;
    int    count = 0;

    do {
        double F[dim], JF[dim][dim], HF[dim][dim][dim];
        double G[dim], JG[dim][dim], HG[dim][dim][dim];
        double A[dim][dim];
        double b[dim];

        fluxFunction_->fill_with_jet(dim, U, 2, F, &JF[0][0], &HF[0][0][0]);
        accFunction_->fill_with_jet(dim, U, 2, G, &JG[0][0], &HG[0][0][0]);

        std::vector<eigenpair> e;
        Eigen::eig(dim, &JF[0][0], &JG[0][0], e);
        // The eigenvalue to use is e[type].r
        double sigma = e[type].r;

        // Some auxiliary number used in system Ax = b
        //

        // For Rankine-Hugoniot condition
        double f1Bracket = F[0] - F_ref.component(0);
        double f2Bracket = F[1] - F_ref.component(1);
        double g1Bracket = G[0] - G_ref.component(0);
        double g2Bracket = G[1] - G_ref.component(1);

        // For det(lambda*JG - JF)
        double det11Bracket = sigma * JG[0][0] - JF[0][0];
        double det12Bracket = sigma * JG[0][1] - JF[0][1];
        double det21Bracket = sigma * JG[1][0] - JF[1][0];
        double det22Bracket = sigma * JG[1][1] - JF[1][1];

        /* TODO: Para o caso de dimensao generica, deve ser alguma coisa como:
            // For Rankine-Hugoniot condition, flux and accumulation brackets
            double fBracket[dim], gBracket[dim];
            // For eigenvalue determinant the characteristic matrix, the derivatives are store inside the
            // auxiliary tensor dCM.
            double CharMatrix[dim][dim], dCM[dim][dim][dim];

            for (int i = 0; i < dim; i++) {
                fBracket[i] = F[i] - F_ref.component(i);
                gBracket[i] = G[i] - G_ref.component(i);
                for (int j = 0; j < dim; j++) {
                    CharMatrix[i][j] = sigma*JG[i][j] - JF[i][j];
                    for (int k = 0; k < dim; k++) {
                        dCM[i][j][k] = sigma*HG[i][j][k] - HF[i][j][k];
                    }
                }
            }

         */

        // The minus sign is incorporated within the parentesis
        b[0] = 2 * (f1Bracket * g2Bracket - f2Bracket * g1Bracket);
        b[1] = 2 * (det12Bracket * det21Bracket - det11Bracket * det22Bracket);

        for (int j = 0; j < dim; j++) {
            A[0][j] = JG[0][j] * f2Bracket + g1Bracket * JF[1][j]
                    - JG[1][j] * f1Bracket - g2Bracket * JF[0][j];
            A[1][j] = (sigma * HG[0][0][j] - HF[0][0][j]) * det22Bracket
                    + (sigma * HG[1][1][j] - HF[1][1][j]) * det11Bracket
                    - (sigma * HG[0][1][j] - HF[0][1][j]) * det21Bracket
                    - (sigma * HG[1][0][j] - HF[1][0][j]) * det12Bracket;
        }

        /* TODO: Para o caso de dimensao generica, deve ser alguma coisa como:
           TODO: Mudei a ordem do que esta escrito acima...
            b[0] = -2*det(CharMatrix);
            for (int i = 1; i < dim; i++) {
                b[i] = 2*(fBracket[0]*gBracket[i] - fBracket[i]*gBracket[0]);
            }
         */

        double det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
        anorm = 0.0;

        // Only dim = 2 is here.
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                anorm += A[i][j] * A[i][j];
            }
        }

        // If Newton does not converge, return the original point.
        if (fabs(det) <= (epsilon * anorm)) {
            cout << "ColorCurve::Right_Newton does not converge." << endl;
            cout << "count = " << count << " " << input << endl;
            for (int i = 0; i < dim; i++) out.component(i) = input.component(i);
            return;
        }

        // Protect against zero-division or use LAPACK (will be done so anyway for n >= 3)
        delta_U[0] = (b[0] * A[1][1] - b[1] * A[0][1]) / det;
        delta_U[1] = (A[0][0] * b[1] - A[1][0] * b[0]) / det;

        // Prepare next step:
        for (int i = 0; i < dim; i++) U[i] += delta_U[i];

        deltaNorm = 0.0;
        for (int i = 0; i < dim; i++) deltaNorm += (delta_U[i] * delta_U[i]);
        count++;
    } while (deltaNorm > epsilon2  && count < 100);

    cout << "Saindo de RN(" << count << "). Input = " << input << ", U = " << U[0]<<" "<<U[1] << endl;

    // Output
    for (int i = 0; i < dim; i++) out.component(i) = U[i];

    return;
}

int ColorCurve::interpolate(const RealVector &p, double &s_p,
        const std::vector<double> &eigenvalue_p, const int type_p,
        const RealVector &q, double &s_q,
        const std::vector<double> &eigenvalue_q, const int type_q,
        vector<RealVector> &r, vector<int> &rtype) {

    // The number of inequalities show the kind of inequality... zero is an error.
    int abs_ineq = abs(type_p - type_q);
    if (abs_ineq == 0) return INTERPOLATION_ERROR;
    int increase = (type_p - type_q < 0 ? 1 : -1);

    int dim = p.size();
    int fam = eigenvalue_p.size();

    double fp, fq; // f(p), f(q), the function whose zero is to be found
    std::vector<double> alpha;

    // Number of inequalities
    int noi = 0;

    // Find where the sign changes.
    // The loop is implemented so a different number of families may be used.
    //
    for (int i = 0; i < fam; i++) {
        if (abs_ineq % 2) {
            fp = ref_eigenvalue[i] - s_p;
            fp = ref_eigenvalue[i] - s_q;
            alpha.push_back(fq / (fq - fp));
            rtype.push_back(increase);
            noi++;
        }
        abs_ineq /= 2;
        increase *= 2;

        // Here the alphas are orginized in increasing order
        // (Thus alpha[0] will be closer to zero, and r[noi-1] closer one).        
        if (noi > 1) {
            for (int j = noi; j > 0; j--) {
                if (alpha[j] < alpha[j - 1]) {
                    double temp = alpha[j];
                    int rtemp = rtype[j];
                    alpha[j] = alpha[j - 1];
                    alpha[j] = temp;
                    rtype[j] = rtype[j - 1];
                    rtype[j] = rtemp;
                } else break;
            }
        }
    }

    for (int i = 0; i < fam; i++) {
        if (abs_ineq % 2) {
            fp = eigenvalue_p[i] - s_p;
            fp = eigenvalue_q[i] - s_q;
            alpha.push_back(fq / (fq - fp));
            rtype.push_back(increase);
            noi++;
        }
        abs_ineq /= 2;
        increase *= 2;

        // Here the alphas are orginized in increasing order
        // (Thus alpha[0] will be closer to zero, and r[noi-1] closer to one).        
        if (noi > 1) {
            for (int j = noi; j > 0; j--) {
                if (alpha[j] < alpha[j - 1]) {
                    double temp = alpha[j];
                    int rtemp = rtype[j];
                    alpha[j] = alpha[j - 1];
                    alpha[j] = temp;
                    rtype[j] = rtype[j - 1];
                    rtype[j] = rtemp;
                } else break;
            }
        }
    }

    // Here the alphas are orginized in increasing order
    // (Thus alpha[0] will be closer to zero, and r[noi-1] closer one).        
    std::sort(alpha.begin(), alpha.end());

    // If the sign changes more than 2 times the number of families, return an error.
    if (noi <= 0 || noi > 2 * fam) return INTERPOLATION_ERROR;
    else {
        // Because of the ordering, r[0] is closer to p and r[noi-1] closer to q.
        r.resize(noi);
        for (int i = 0; i < noi; i++) {
            r[i].resize(p.size());
            for (int j = 0; j < p.size(); j++) r[i].component(j) =
                    (1 - alpha[i]) * p.component(j) + alpha[i] * q.component(j);
        }

        return INTERPOLATION_OK;
    }
}

int ColorCurve::complete_point(RealVector &p, double &s, std::vector<double> &eigenvalue) {
    int dim = p.size();
    
    // F, JF, G & JG at p:
    //
    double F[dim], G[dim], JF[dim][dim], JG[dim][dim];
    
    fluxFunction_->fill_with_jet(dim, p.components(), 1, F, &JF[0][0], 0);
    accFunction_->fill_with_jet(dim, p.components(), 1, G, &JG[0][0], 0);

    // Eigenvalues.
    //
    std::vector<eigenpair> e;
    Eigen::eig(dim, &JF[0][0], &JG[0][0], e);
    eigenvalue.resize(e.size());
    for (int i = 0; i < e.size(); i++) eigenvalue[i] = e[i].r;

    // Speed.
    //
    double num = 0.0, den = 0.0;

    for (int i = 0; i < dim; i++) {
        double delta_G = G[i] - G_ref[i];

        num += (F[i] - F_ref[i]) * delta_G;

        den += delta_G*delta_G;
        
         cout<<" G:"<<G[i]<<" F:"<<F[i]<<" Fref:"<<F_ref[i]<<" Gref:"<<G_ref[i]<<endl;
    }

    if (fabs(den) > 1e-20) s = num / den;
    else {
        cout<<"No else"<<endl;
        // The speed is just approximated. TODO: This is not a good idea, something is needed.
        s = num * 1e20;
        return UNCLASSIFIABLE_POINT;
    }

    // NOTICE that zero must be the initial classification of such a point.
    //
    return 0;
}

int ColorCurve::classify_point(RealVector &p, double &s, std::vector<double> &eigenvalue) {

    // In order to classify the point, speed and eigenvalues are needed. They are filled in complete:
    // (type is filled as 0 when the classification is possible.)
    //
    int type = complete_point(p, s, eigenvalue);
    if (type == UNCLASSIFIABLE_POINT) return UNCLASSIFIABLE_POINT;

    int increment = 1;

    // The classification changes with number of families, the first increments (at reference point)
    // are +1, +2, ..., +2^(fam-1) and the second increments (at the current point) are given with
    // the next values, i.e., +2^fam, +2^(fam+1), ..., +2^(2*fam-1).
    //
    // ****  A T E N T I O N:  *****
    // Further atention in handling the coloring table for number of families is needed outside.
    //
//    for (int fam = 0; fam < ref_eigenvalue.size(); fam++) {
//        // Assing increments for left side (reference point)
//        if (ref_eigenvalue[fam] - s > 0.0) type += increment;
//        increment *= 2;
//    }
//
//    for (int fam = 0; fam < ref_eigenvalue.size(); fam++) {
//        // Assing increments for right side (current point)
//        if (eigenvalue[fam] - s > 0.0) type += increment;
//        increment *= 2;
//    }
    

     for (int fam = 0; fam < ref_eigenvalue.size(); fam++) {
        // Assing increments for left side (reference point)
        if (ref_eigenvalue[fam] - s > 0.0) {
            type += increment;

        }
        increment *= 2;
    }

    for (int fam = 0; fam < ref_eigenvalue.size(); fam++) {
        // Assing increments for right side (current point)
        if (eigenvalue[fam] - s > 0.0){
            type += increment;
        }
        increment *= 2;
    }

    return type;
}

void ColorCurve::classify_segment(RealVector &p, RealVector &q,
        std::vector<HugoniotPolyLine> &classified_curve,
        std::vector<RealVector> &transition_list) {

    double s_p, s_q;
    std::vector<double> eigenvalue_p, eigenvalue_q;

    int type_p = classify_point(p, s_p, eigenvalue_p);
    int type_q = classify_point(q, s_q, eigenvalue_q);

    int dim = p.size();
    HugoniotPolyLine hpl;

    // One of the points (or both) could not be classfied. (Output as is.)
    //
    if (type_p == UNCLASSIFIABLE_POINT || type_q == UNCLASSIFIABLE_POINT) {
        hpl.type = UNCLASSIFIABLE_POINT;

        hpl.point.resize(2);
        hpl.point[0].resize(dim);
        hpl.point[1].resize(dim);

        for (int j = 0; j < dim; j++) {
            hpl.point[0].component(j) = p.component(j);
            hpl.point[1].component(j) = q.component(j);
        }

        // Unclassifications occurs because of an error in the speed, these values are not trustable.
        //
        hpl.speed.resize(2);
        hpl.speed[0] = s_p;
        hpl.speed[1] = s_q;

        hpl.eigenvalue.resize(2);
        int fam = eigenvalue_p.size();
        hpl.eigenvalue[0].resize(fam);
        hpl.eigenvalue[1].resize(fam);

        for (int j = 0; j < fam; j++) {
            hpl.eigenvalue[0].component(j) = eigenvalue_p[j];
            hpl.eigenvalue[1].component(j) = eigenvalue_q[j];
        }

        classified_curve.push_back(hpl);
    }// Both points share the same type (the output segment will not be divided).
        //
    else if (type_p == type_q) {
        hpl.type = type_p;

        hpl.point.resize(2);
        hpl.point[0].resize(dim);
        hpl.point[1].resize(dim);

        for (int j = 0; j < dim; j++) {
            hpl.point[0].component(j) = p.component(j);
            hpl.point[1].component(j) = q.component(j);
        }

        hpl.speed.resize(2);
        hpl.speed[0] = s_p;
        hpl.speed[1] = s_q;

        hpl.eigenvalue.resize(2);
        int fam = eigenvalue_p.size();
        hpl.eigenvalue[0].resize(fam);
        hpl.eigenvalue[1].resize(fam);

        for (int j = 0; j < fam; j++) {
            hpl.eigenvalue[0].component(j) = eigenvalue_p[j];
            hpl.eigenvalue[1].component(j) = eigenvalue_q[j];
        }

        classified_curve.push_back(hpl);
    }// Points have different classification, splitting is needed.
        //
    else {
        // The number of families do not change inside.
        int fam = eigenvalue_p.size();
        // This value is for closeness proposes.
        double epsilon = 0.0;
        for (int i = 0; i < dim; i++) epsilon +=
                (p.component(i) - q.component(i))*(p.component(i) - q.component(i));
        epsilon = 0.000005 * sqrt(epsilon); // TODO: Devemos estimar o valor certo.

        // Auxiliary vectors to store data.
        //
        std::vector<RealVector> r, rtemp;
        r.clear();
        rtemp.clear();
        std::vector<int> rtype, rttemp;
        rtype.clear();
        rttemp.clear();
        std::vector<double> rspeed;
        rspeed.clear();
        std::vector< std::vector<double> > reigen;
        reigen.clear();

        // Organize and store values. (The extra points are orginized from p to q.)
        //
        interpolate(p, s_p, eigenvalue_p, type_p, q, s_q, eigenvalue_q, type_q, rtemp, rttemp);
        int noi = rtemp.size();
        int index = 0;
        double distance, d_temp;
        double r_p;
        std::vector<double> eigenvalue_r;

        // Improve the new points to be on the Hugoniot locus. Store in transition_list.
        // (Only when number of equations equals the dimension .)
        //
        if (dim == fam) {
            for (int i = 0; i < noi; i++) {
                // The next loop controls the zero to look for, zerotype in [0, fam - 1] is the
                // zerotype-th left family which is zero, for zerotype in [fam, 2*fam - 1] is the
                // (zerotype - fam)-th right family which is zero.
                //
                int ztype = abs(rttemp[i]);
                int zerotype = 0;
                while (!(ztype % 2)) {
                    zerotype++;
                    ztype /= 2;
                }
                cout << "O tipo de zero eh: " << zerotype << ", desde: " << rttemp[i] << endl;
                RealVector out;
                if (zerotype < fam) Left_Newton_improvement(rtemp[i], zerotype, out);
                else Right_Newton_improvement(rtemp[i], zerotype - fam, out);
                rtemp[i] = out;
            }
        }

        // Fill the segments will all data. Recall that rt(ype)temp is marked only with the zero
        // signature, here we complete the real signature of the point...
        //
        r.push_back(p);
        rtype.push_back(type_p);
        rspeed.push_back(s_p);
        reigen.push_back(eigenvalue_p);
        int signature = rtype[0];
        for (int k = 0; k < noi; k++) {
            signature += rttemp[k];

            // When the new point is closed the old point, this is removed from list. The signature
            // is the adding of both.
            distance = 0.0;
            for (int j = 0; j < dim; j++) {
                d_temp = r[index].component(j) - rtemp[k].component(j);
                distance += d_temp * d_temp;
            }
            if (distance < epsilon) continue;
            // This index is usefull for the distance above.
            index += 1;

            // If not removed, the transition list increases
            transition_list.push_back(rtemp[k]);

            complete_point(rtemp[k], r_p, eigenvalue_r);
            r.push_back(rtemp[k]);
            rtype.push_back(signature);
            rspeed.push_back(r_p);
            reigen.push_back(eigenvalue_r);
        }
        r.push_back(q);
        rtype.push_back(type_q);
        rspeed.push_back(s_q);
        reigen.push_back(eigenvalue_q);
        //
        // Actually signature is the same as type_q.

        // Actualize the number of intersections and add the initial point
        noi = r.size() - 1;

        // Now we fill the new segment and its components for the HugoniotPolyLine
        //
        for (int k = 0; k < noi; k++) {
            hpl.type = rtype[k];

            hpl.point.resize(2);
            hpl.point[0].resize(dim);
            hpl.point[1].resize(dim);

            for (int j = 0; j < dim; j++) {
                hpl.point[0].component(j) = r[k].component(j);
                hpl.point[1].component(j) = r[k + 1].component(j);
            }

            hpl.speed.resize(2);
            hpl.speed[0] = rspeed[k];
            hpl.speed[1] = rspeed[k + 1];

            hpl.eigenvalue.resize(2);
            hpl.eigenvalue[0].resize(fam);
            hpl.eigenvalue[1].resize(fam);

            for (int j = 0; j < fam; j++) {
                hpl.eigenvalue[0].component(j) = reigen[k][j];
                hpl.eigenvalue[1].component(j) = reigen[k + 1][j];
            }
            classified_curve.push_back(hpl);
        }
    }

    return;
}

// This method classify a Hugoniot Locus given as a collection of segments, typically from Contour 
//

void ColorCurve::classify_segmented_curve(std::vector<RealVector > &original,
        const RealVector &ref,
        std::vector<HugoniotPolyLine> &classified_curve,
        std::vector<RealVector> &transition_list) {

    classified_curve.clear();
    transition_list.clear();

    // Get the ref point.
    //
    int dim = ref.size();
    ref_point.resize(dim);

    for (int i = 0; i < dim; i++) ref_point.component(i) = ref.component(i);


    cout << "Ref point em classified: " << ref_point << endl;

    // Eigenvalues at the reference point.
    //
    F_ref.resize(dim);
    G_ref.resize(dim);
    double JF[dim][dim], JG[dim][dim];
    fluxFunction_->fill_with_jet(dim, ref_point.components(), 1, F_ref.components(), &JF[0][0], 0);
    accFunction_->fill_with_jet(dim, ref_point.components(), 1, G_ref.components(), &JG[0][0], 0);

    std::vector<eigenpair> e;
    Eigen::eig(dim, &JF[0][0], &JG[0][0], e);

    ref_eigenvalue.resize(e.size());
    for (int i = 0; i < e.size(); i++) ref_eigenvalue[i] = e[i].r;



    cout << "dim: " << dim << endl;

    // Process the list
    for (int i = 0; i < original.size() / 2; i++) {
        classify_segment(original[2 * i], original[2 * i + 1], classified_curve, transition_list);
    }

    return;
}

// This method classify a Hugoniot Locus given as a collection of points, typically from continuation 
//

void ColorCurve::classify_continuous_curve(std::vector<RealVector> &original,
        const RealVector &ref,
        std::vector<HugoniotPolyLine> &classified_curve,
        std::vector<RealVector> &transition_list) {

    classified_curve.clear();
    transition_list.clear();

    // Get the ref point.
    //
    int dim = ref.size();
    ref_point.resize(dim);
    for (int i = 0; i < dim; i++) ref_point.component(i) = ref.component(i);

    // Eigenvalues at the reference point.
    //
    F_ref.resize(dim);
    G_ref.resize(dim);
    double JF[dim][dim], JG[dim][dim];
    fluxFunction_->fill_with_jet(dim, ref_point.components(), 1, F_ref.components(), &JF[0][0], 0);
    accFunction_->fill_with_jet(dim, ref_point.components(), 1, G_ref.components(), &JG[0][0], 0);

    std::vector<eigenpair> e;
    Eigen::eig(dim, &JF[0][0], &JG[0][0], e);

    ref_eigenvalue.resize(e.size());
    for (int i = 0; i < e.size(); i++) ref_eigenvalue[i] = e[i].r;

    // The first segment is classified with the second point:
    //
    double s1;
    std::vector<double> eigenvalue1;
    int type = classify_point(original[1], s1, eigenvalue1);

    HugoniotPolyLine hpl;
    // This is the first HugoniotPolyLine, storing data
    hpl.type = type;

    hpl.point.resize(2);
    hpl.point[0].resize(dim);
    hpl.point[1].resize(dim);

    for (int j = 0; j < dim; j++) {
        hpl.point[0].component(j) = ref_point.component(j);
        hpl.point[1].component(j) = original[1].component(j);
    }

    hpl.speed.resize(2);
    // TODO: All shock speeds are trivially valid for the reference point, store s1 and thus in JAVA
    //       the shock speed returned for the whole segment is fixed.
    hpl.speed[0] = s1;
    hpl.speed[1] = s1;

    hpl.eigenvalue.resize(2);
    int fam = eigenvalue1.size();



    hpl.eigenvalue[0].resize(fam);
    hpl.eigenvalue[1].resize(fam);

    for (int j = 0; j < fam; j++) {
        hpl.eigenvalue[0].component(j) = ref_eigenvalue[j];
        hpl.eigenvalue[1].component(j) = eigenvalue1[j];
    }

    classified_curve.push_back(hpl);

    // Process the (remaining) list
    for (int i = 1; i < original.size() - 1; i++) {
        classify_segment(original[i], original[i + 1], classified_curve, transition_list);
    }

    return;
}


// TODO: This signature must DIE after removing old codes!!

void ColorCurve::classify_curve(vector < vector < RealVector > > &, const RealVector &, int, int, vector < HugoniotPolyLine > &output) {
    return;
}
