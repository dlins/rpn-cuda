#include "WaveCurvePlot.h"
#include "TestTools.h"

WaveCurvePlot::WaveCurvePlot(const WaveCurve &w, int type, double min_speed, double max_speed, int speed_steps) : GraphicObject(0, 0, 0){
    // Vector of speeds, it will be used to determine where the arrows will be drawn.
    //
//    std::vector<double> speeds;
//    double delta = (max_speed - min_speed)/(double)(speed_steps - 1);
//    for (int i = 0; i < speed_steps; i++) speeds.push_back(min_speed + delta*(double)i);

    std::vector<double> speeds;
    double temp_min_speed = w.wavecurve.front().speed.front();
    double temp_max_speed = w.wavecurve.front().speed.front();
    for (int i = 0; i < w.wavecurve.size(); i++){
        for (int j = 0; j < w.wavecurve[i].speed.size(); j++){
            temp_min_speed = std::min(temp_min_speed, w.wavecurve[i].speed[j]);
            temp_max_speed = std::max(temp_max_speed, w.wavecurve[i].speed[j]);
        }
    }

    double delta = (max_speed - min_speed)/(double)(speed_steps - 1);
    for (int i = 0; i < speed_steps; i++) speeds.push_back(min_speed + delta*(double)i);

//    // Use the wavecurve's maximum and minimum speeds, not the ones provided by the user.
//    //
//    double delta = (temp_max_speed - temp_min_speed)/(double)(speed_steps - 1);
//    for (int i = 0; i < speed_steps; i++) speeds.push_back(temp_min_speed + delta*(double)i);

    Curve2D *c;

    // Add the curves.
    //
    for (int i = 0; i < w.wavecurve.size(); i++){
        double r = 0.0, g = 0.0, b = 0.0;

        if (w.wavecurve[i].type == RAREFACTION_CURVE) r = 1.0;
        else if (w.wavecurve[i].type == SHOCK_CURVE)  b = 1.0;
        else                                          g = 1.0;

        std::vector<Point2D> p;
        for (int j = 0; j < w.wavecurve[i].curve.size(); j++) p.push_back(Point2D(w.wavecurve[i].curve[j](0), w.wavecurve[i].curve[j](1)));

        c = new Curve2D(w.wavecurve[i].curve, r, g, b, CURVE2D_SOLID_LINE | CURVE2D_ARROWS);
//        c = new Curve2D(p, r, g, b, CURVE2D_SOLID_LINE | CURVE2D_ARROWS);

        std::vector<int> arrow_tip;
        std::vector<int> arrow_base;

        const std::vector<double> &current_curve_speed = w.wavecurve[i].speed;
        
        for (int j = 0; j < current_curve_speed.size() - 1; j++){
            for (int k = 0; k < speeds.size(); k++){
               if ((current_curve_speed[j] - speeds[k])*(current_curve_speed[j + 1] - speeds[k]) < 0.0){ 
                    arrow_tip.push_back(j);

                    if (current_curve_speed[j] < current_curve_speed[j + 1]) arrow_base.push_back(j - 1);
                    else                                                     arrow_base.push_back(j + 1);
                }
            }

//            // Uniform distribution of the arrows, not taking into account the variations in speed.
//            // Useful for tests.
//            //
//                if (j % 100 == 0){
//                    arrow_head_positions.push_back(j);
//                    arrow_head_directions.push_back(CURVE2D_ARROW_POINTS_NEXT);
//                }
        }

        c->set_arrowheads(arrow_tip, arrow_base);

        sons.push_back(c);
    }

    // Add origin.
    //
    c = new Curve2D(w.wavecurve.front().curve.front(), 0.0, 0.0, 0.0, CURVE2D_MARKERS);
    sons.push_back(c);
}

WaveCurvePlot::WaveCurvePlot(const WaveCurve &w, const RealVector &orig, int type) : GraphicObject(0, 0, 0){
    Curve2D *c;

    // Add curves
    for (int i = 0; i < w.wavecurve.size(); i++){
        double r = 0.0, g = 0.0, b = 0.0;

        if (w.wavecurve[i].type == RAREFACTION_CURVE) r = 1.0;
        else if (w.wavecurve[i].type == SHOCK_CURVE)  b = 1.0;
        else                                          g = 1.0;

        std::vector<Point2D> p;
       
        if (w.wavecurve[i].compatible.size() == 0){
            for (int j = 0; j < w.wavecurve[i].curve.size(); j++) p.push_back(Point2D(w.wavecurve[i].curve[j].component(0), w.wavecurve[i].curve[j].component(1)));
        }
        else {
            for (int j = 0; j < w.wavecurve[i].curve.size(); j++){
                if (w.wavecurve[i].compatible[j]) p.push_back(Point2D(w.wavecurve[i].curve[j].component(0), w.wavecurve[i].curve[j].component(1)));
            }
        }

//        for (int j = 0; j < w.wavecurve[i].curve.size(); j++){
//            if (w.wavecurve[i].)
//            p.push_back(Point2D(w.wavecurve[i].curve[j].component(0), w.wavecurve[i].curve[j].component(1)));
//        }

//        if (w[i].type == COMPOSITE_CURVE) c = new Curve2D(p, r, g, b, CURVE2D_SOLID_LINE | CURVE2D_MARKERS | CURVE2D_INDICES);
//        else c = new Curve2D(p, r, g, b, CURVE2D_SOLID_LINE);

        c = new Curve2D(p, r, g, b, type);

        sons.push_back(c);

        // Add transition points
        std::cout << "WaveCurvePlot. i = " << i << ", transition points = " << w.wavecurve[i].explicit_bifurcation_transition_index.size() << std::endl;
        std::vector<RealVector> tp;
        for (int j = 0; j < w.wavecurve[i].explicit_bifurcation_transition_index.size(); j++){
            int pos = w.wavecurve[i].explicit_bifurcation_transition_index[j];

            std::cout << "    j = " << j << ", pos = " << pos << std::endl;
            if (pos < w.wavecurve[i].curve.size()) tp.push_back(w.wavecurve[i].curve[pos]);
        }

        if (tp.size() > 0){
            c = new Curve2D(tp, r, g, b, CURVE2D_MARKERS);
            sons.push_back(c);
        }
    }

    // Add origin
    c = new Curve2D(w.wavecurve.front().curve.front(), 0.0, 0.0, 0.0, CURVE2D_MARKERS);
    sons.push_back(c);
}

WaveCurvePlot::WaveCurvePlot(const WaveCurve &w, const RealVector &orig, const std::vector<std::vector<std::string> > &str, int type) : GraphicObject(0, 0, 0){
    Curve2D *c;

    // Add curves
    for (int i = 0; i < w.wavecurve.size(); i++){
        double r = 0.0, g = 0.0, b = 0.0;

        if (w.wavecurve[i].type == RAREFACTION_CURVE) r = 1.0;
        else if (w.wavecurve[i].type == SHOCK_CURVE)  b = 1.0;
        else                                g = 1.0;

        std::vector<Point2D> p;
       
        for (int j = 0; j < w.wavecurve[i].curve.size(); j++) p.push_back(Point2D(w.wavecurve[i].curve[j].component(0), w.wavecurve[i].curve[j].component(1)));

//        if (w[i].type == COMPOSITE_CURVE) c = new Curve2D(p, r, g, b, CURVE2D_SOLID_LINE | CURVE2D_MARKERS | CURVE2D_INDICES);
//        else c = new Curve2D(p, r, g, b, CURVE2D_SOLID_LINE);

        c = new Curve2D(p, r, g, b, str[i], type);

        sons.push_back(c);

        // Add transition points
        std::cout << "WaveCurvePlot. i = " << i << ", transition points = " << w.wavecurve[i].explicit_bifurcation_transition_index.size() << std::endl;
        std::vector<RealVector> tp;
        for (int j = 0; j < w.wavecurve[i].explicit_bifurcation_transition_index.size(); j++){
            tp.push_back(w.wavecurve[i].curve[w.wavecurve[i].explicit_bifurcation_transition_index[j]]);
        }
        c = new Curve2D(tp, r, g, b, CURVE2D_MARKERS);
        sons.push_back(c);
    }

    // Add origin
    std::vector<Point2D> p;
    p.push_back(Point2D(orig.component(0), orig.component(1)));

    c = new Curve2D(p, 0.0, 0.0, 0.0, CURVE2D_MARKERS);
    sons.push_back(c);
}

//WaveCurvePlot::WaveCurvePlot(const WaveCurve &w, const RealVector &orig, Canvas *canvas, CanvasMenuScroll *scroll, int type) : GraphicObject(0, 0, 0){
//    Curve2D *c;

//    std::vector<std::string> type_string;
//    type_string.push_back(std::string("Rarefaction"));
//    type_string.push_back(std::string("Shock"));
//    type_string.push_back(std::string("Composite"));

//    // Add curves
//    for (int i = 0; i < w.wavecurve.size(); i++){
//        double r = 0.0, g = 0.0, b = 0.0;

//        if (w.wavecurve[i].type == RAREFACTION_CURVE) r = 1.0;
//        else if (w.wavecurve[i].type == SHOCK_CURVE)  b = 1.0;
//        else                                          g = 1.0;

//        std::vector<Point2D> p;
//       
//        for (int j = 0; j < w.wavecurve[i].curve.size(); j++) p.push_back(Point2D(w.wavecurve[i].curve[j].component(0), w.wavecurve[i].curve[j].component(1)));

////        if (w[i].type == COMPOSITE_CURVE) c = new Curve2D(p, r, g, b, CURVE2D_SOLID_LINE | CURVE2D_MARKERS | CURVE2D_INDICES);
////        else c = new Curve2D(p, r, g, b, CURVE2D_SOLID_LINE);

//        c = new Curve2D(p, r, g, b, type);

//        sons.push_back(c);
//        canvas->add(c);

//        std::stringstream ss;
//        ss << type_string[w.wavecurve[i].type - 1] << ", size = " << w.wavecurve[i].curve.size();
//        scroll->add(ss.str().c_str(), canvas, c);
//    }

//    // Add origin
//    std::vector<Point2D> p;
//    p.push_back(Point2D(orig.component(0), orig.component(1)));

//    c = new Curve2D(p, 0.0, 0.0, 0.0, CURVE2D_MARKERS);
//    sons.push_back(c);
//}

WaveCurvePlot::~WaveCurvePlot(){
    for (int i = 0; i < sons.size(); i++) delete sons[i];
}

void WaveCurvePlot::transform(const double *matrix){
    for (int i = 0; i < sons.size(); i++) sons[i]->transform(matrix);
    return;
}

void WaveCurvePlot::draw(){
    for (int i = 0; i < sons.size(); i++) sons[i]->draw();
    return;
}

void WaveCurvePlot::minmax(Point2D &, Point2D &){
    return;
}

