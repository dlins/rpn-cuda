/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn;

import java.awt.Color;
import java.awt.event.MouseEvent;
import wave.multid.view.*;
import java.awt.Graphics2D;
import java.awt.print.Printable;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Shape;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Line2D;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.ToolTipManager;
import rpn.component.ClosestDistanceCalculator;
import rpn.component.DiagramGeom;
import rpn.parser.RPnDataModule;
import rpnumerics.Diagram;
import wave.multid.Coords2D;
import wave.multid.CoordsArray;
import wave.multid.Space;
import wave.multid.graphs.ViewPlane;
import wave.multid.graphs.dcViewport;
import wave.multid.graphs.wcWindow;
import wave.util.RealSegment;
import wave.util.RealVector;

public class RPnRiemannPanel extends RPnPhaseSpacePanel implements Printable {
    //
    // Constants
    //

    //
    // Members
    //
    private Point cursorPos_;
    private List<Point> trackedPointList_;
    private Line2D.Double trackLine_;

    private Scene scene_;

    //
    // Constructors
    //
    public RPnRiemannPanel(Scene scene) {

        scene_ = scene;
        cursorPos_ = new Point(0, 0);
        trackLine_ = new Line2D.Double(cursorPos_, cursorPos_);
        trackedPointList_ = new ArrayList<Point>();

        addMouseMotionListener(new MouseMotionHandler());
        addMouseListener(new MouseListenerHandler());
        addComponentListener(new PanelSizeController());

    }

    @Override
    public Scene scene() {
        return scene_;
    }

    //
    // Accessors/Mutators
    //
    //
    // Methods
    //
    private class MouseMotionHandler implements MouseMotionListener {

        @Override
        public void mouseDragged(MouseEvent me) {
////            throw new UnsupportedOperationException("Not supported yet.");
        }

        @Override
        public void mouseMoved(MouseEvent me) {

            trackedPointList_.clear();
            cursorPos_ = me.getPoint();

            trackLine_ = new Line2D.Double(cursorPos_.x, 0, cursorPos_.x, getHeight());
            Iterator geomObjIterator = RPnDataModule.RIEMANNPHASESPACE.getGeomObjIterator();

            ViewingTransform transform = scene_.getViewingTransform();

            while (geomObjIterator.hasNext()) {
                DiagramGeom diagram = (DiagramGeom) geomObjIterator.next();

                Coords2D cursorPoint = new Coords2D(cursorPos_.getX(), cursorPos_.getY());

                CoordsArray cursorWC = new CoordsArray(new Space("", 2));

                transform.dcInverseTransform(cursorPoint, cursorWC);

                Diagram d = (Diagram) diagram.geomFactory().geomSource();

                for (int i = 0; i < d.getLines().size(); i++) {

                    List<RealSegment> segments = d.getLine(i).getSegments();

                    // Teste closest Point                
                    RealVector pointTest = new RealVector(2);

                    pointTest.setElement(0, cursorWC.getElement(0));
                    pointTest.setElement(1, 0);

                    ClosestDistanceCalculator calculator = new ClosestDistanceCalculator(segments, pointTest);

                    Coords2D testeDCCoords = new Coords2D(0, 0);

                    CoordsArray testeWCCoords = new CoordsArray(calculator.getClosestPoint());

                    transform.viewPlaneTransform(testeWCCoords, testeDCCoords);

                    trackedPointList_.add(new Point((int) testeDCCoords.getX(), (int) testeDCCoords.getY()));

                }

                repaint();

            }
        }
    }

    private class PanelSizeController extends ComponentAdapter {

        @Override
        public void componentResized(ComponentEvent event) {

            if (event.getComponent() instanceof RPnPhaseSpacePanel) {

                RPnPhaseSpacePanel panel = (RPnPhaseSpacePanel) event.getComponent();

                int wPanel = panel.getWidth();
                int hPanel = panel.getHeight();

                dcViewport newViewport = new dcViewport(wPanel, hPanel);
                wcWindow currWindow = panel.scene().getViewingTransform().viewPlane().getWindow();

                panel.scene().getViewingTransform().setViewPlane(new ViewPlane(newViewport, currWindow));
                panel.scene().update();
                panel.repaint();
            }

        }
    }

    private class MouseListenerHandler implements MouseListener {

        @Override
        public void mouseClicked(MouseEvent me) {
//            throw new UnsupportedOperationException("Not supported yet.");
        }

        @Override
        public void mousePressed(MouseEvent me) {

        }

        @Override
        public void mouseReleased(MouseEvent me) {
//            throw new UnsupportedOperationException("Not supported yet.");
        }

        @Override
        public void mouseEntered(MouseEvent me) {
//            throw new UnsupportedOperationException("Not supported yet.");
        }

        @Override
        public void mouseExited(MouseEvent me) {
//            throw new UnsupportedOperationException("Not supported yet.");
        }
    }

    @Override
    public void paintComponent(Graphics g) {

        /*
         * BOUNDARY WINDOW
         */
        g.setColor(DEFAULT_BACKGROUND_COLOR);
        Shape s = scene().getViewingTransform().viewPlane().getWindow().dcView(scene().getViewingTransform());
        ((Graphics2D) g).fill(s);

        g.setColor(Color.yellow);

        for (Point trackedPoint : trackedPointList_) {
            g.drawRect(trackedPoint.x, trackedPoint.y, 2, 2);

        }


        /*
         * SCENE
         */
        if (scene() != null) {

            scene().draw((Graphics2D) g);
        }

        g.setColor(Color.red);
        g.drawLine((int) trackLine_.x1, (int) trackLine_.y1, (int) trackLine_.x2, (int) trackLine_.y2);
    }
    
    

    public List<Point> getTrackedPointList() {
        return trackedPointList_;
    }

}
