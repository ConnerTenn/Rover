/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package marsrover;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTabbedPane;

/**
 *
 * @author Bradley
 */
public class MainGUI extends JFrame {

    protected static final int LENGTH = 1000;
    protected static final int WIDTH = 500;
    
    private JPanel mainPanel, clawPanel, videoPanel, lidarPanel;
    private JTabbedPane tabs;
    MainGUI() {
        createWindow();
    }
    
    private void createWindow() {
        setTitle("Rover");
        setSize(LENGTH,WIDTH);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainPanel = new JPanel();
        add(mainPanel);
        clawPanel = new JPanel();
        videoPanel = new JPanel();
        lidarPanel = new JPanel();
        tabs = new JTabbedPane();
        tabs.setTabLayoutPolicy(JTabbedPane.WRAP_TAB_LAYOUT);
        tabs.addTab("Claw", null, clawPanel, "Claw stuff");
        tabs.setMnemonicAt(0, KeyEvent.VK_1);
        tabs.addTab("Video Feed", null, videoPanel, "Video stuff");
        tabs.setMnemonicAt(1, KeyEvent.VK_2);
        tabs.addTab("Lidar", null, lidarPanel, "Lidar stuff");
        tabs.setMnemonicAt(2, KeyEvent.VK_3);
        mainPanel.add(tabs);
        setVisible(true);
    }
}
