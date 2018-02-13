/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package marsrover;

import java.awt.GridLayout;
import java.awt.event.KeyEvent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

/**
 *
 * @author Bradley
 */
public class MainGUI extends JFrame {

    protected static final int LENGTH = 1000;
    protected static final int WIDTH = 500;
    
    private TabbedPane tabs;
    private ClawArialCanvas clawArialCanvas;
    private ClawSideCanvas clawSideCanvas;
    
    MainGUI() {
        createWindow();
    }
    
    private void createWindow() {
        setTitle("Rover        mainPanel.add(tabs);\n" + "");
        setSize(LENGTH,WIDTH);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //setLayout(new GridLayout(1,1));
        
        tabs = new TabbedPane();
        clawArialCanvas = new ClawArialCanvas();
        clawSideCanvas = new ClawSideCanvas();
        add(tabs);

        tabs.getClawPanel().add(clawArialCanvas);
        tabs.getClawPanel().add(clawSideCanvas);
        
        setVisible(true);
    }
}
