/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn;

import java.awt.*;
import java.beans.PropertyChangeEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.Set;
import javax.swing.*;
import rpn.parser.PhysicsProfile;
import rpn.parser.RPnInterfaceParser;
import rpnumerics.FluxParams;
import rpnumerics.RPNUMERICS;
import wave.util.RealVector;

public class RPnFluxParamsPanel extends JPanel {

    private GridBagLayout gridLayout = new GridBagLayout();
    private ArrayList<JTextField> valuesArray_ = new ArrayList<JTextField>();
    private PhysicsProfile physicsProfile_;

    public RPnFluxParamsPanel() {

        searchPhysics(RPNUMERICS.physicsID());
        buildPanel(false);
    }

    public RPnFluxParamsPanel(String physicsName) {
        searchPhysics(physicsName);
        buildPanel(true);
    }

    private void buildPanel(boolean useDefaults) {
        this.setLayout(gridLayout);

        GridBagConstraints gridConstraints = new GridBagConstraints();

        gridConstraints.ipadx = 40;

        ArrayList<HashMap<String, String>> fluxParamsArrayList = physicsProfile_.getFluxParamArrayList();

        for (int i = 0; i < fluxParamsArrayList.size(); i++) {
            HashMap<String, String> fluxParams = fluxParamsArrayList.get(i);

            Set<Entry<String, String>> fluxParamsSet = fluxParams.entrySet();

            Iterator<Entry<String, String>> paramsIterator = fluxParamsSet.iterator();

            while (paramsIterator.hasNext()) {
                Entry<String, String> entry = paramsIterator.next();
                JLabel fluxParamName = new JLabel(entry.getKey());
                fluxParamName.setName(entry.getKey());
                JTextField fluxValueField = new JTextField();
                valuesArray_.add(i, fluxValueField);
                if (useDefaults) {
                    Double paramValue = new Double(entry.getValue());
//                    fluxValueField.setText(entry.getValue());
                    fluxValueField.setText(paramValue.toString());
                } else {

                    FluxParams fluxParam = RPNUMERICS.getFluxParams();
                    Double paramValue = fluxParam.getElement(i);
                    valuesArray_.get(i).setText(paramValue.toString());

                }
                fluxValueField.setName(entry.getKey());

                gridConstraints.gridx = 0;

                this.add(fluxParamName, gridConstraints);
                gridConstraints.gridx = 1;
                this.add(fluxValueField, gridConstraints);
            }
        }
    }

    private void searchPhysics(String physicsName) {

        Iterator<PhysicsProfile> physics = RPnInterfaceParser.getPhysicsProfiles().iterator();

        while (physics.hasNext()) {
            PhysicsProfile physicsProfile = physics.next();
            if (physicsProfile.getName().equals(physicsName)) {
                physicsProfile_ = physicsProfile;
            }
        }

    }

    public void applyParams() {

        StringBuffer paramsBuffer = new StringBuffer();

        for (int i = 0; i < valuesArray_.size(); i++) {
            JTextField jTextField = valuesArray_.get(i);
            paramsBuffer.append(jTextField.getText());
            paramsBuffer.append(" ");
        }

        RealVector paramsVector = new RealVector(paramsBuffer.toString());
        RPNUMERICS.setFluxParams(new FluxParams(paramsVector));
        Integer teste = new Integer(0);
        Integer teste2 = new Integer(1);
        rpn.usecase.ChangeFluxParamsAgent.instance().applyChange(new PropertyChangeEvent(this, "", teste, teste2));

    }
}
