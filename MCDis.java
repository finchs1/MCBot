import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import org.bukkit.Bukkit;
import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;
import org.bukkit.event.EventHandler;
import org.bukkit.event.EventPriority;
import org.bukkit.event.Listener;
import org.bukkit.event.player.AsyncPlayerChatEvent;
import org.bukkit.plugin.java.JavaPlugin;

public class MCDis extends JavaPlugin implements Listener, Runnable
{
    public void run()
    {
        DatagramSocket s1 = null;
        try {
            s1 = new DatagramSocket(60019);
        } catch (SocketException e) {
            e.printStackTrace();
        }

        while (!done)
        {
            byte[] buffer = new byte[256];
            DatagramPacket req = new DatagramPacket(buffer, 256);
            try {
                s1.receive(req);
            } catch (IOException e) {
                e.printStackTrace();
            }
            msg = new String(buffer).trim();
        }
        s1.close();
    }

    public void onEnable()
    {
        getServer().getPluginManager().registerEvents(this, this);
        try {
            address = Inet4Address.getByName("172.0.0.1");//replace with discord bot server address
        } catch (UnknownHostException e1) {
            e1.printStackTrace();
        }

        Thread t1 = new Thread(this);
        t1.start();
        //Bukkit.getScheduler().runTaskTimerAsynchronously(this, this, 1, 1);
        Bukkit.getScheduler().runTaskTimer(this, new Runnable(){
            public void run()
            {
                if (!msg.isEmpty())
                {
                    Bukkit.getServer().broadcastMessage("[Discord] " + msg);
                    msg = "";
                }
            }
        }, 1, 5);
        Bukkit.getScheduler().runTaskTimer(this, new Runnable(){
            public void run()
            {   
                String tmp = "";
                for (Player p : Bukkit.getServer().getOnlinePlayers())
                {
                    tmp += p.getName();
                    tmp += '*';
                }

                DatagramSocket socket = null;

                try {
                    socket = new DatagramSocket();
                } catch (SocketException es) {
                    es.printStackTrace();
                }

                byte[] buf = new byte[257];
                for (int i = 0; i < tmp.getBytes().length; i++)
                    buf[i + 1] = tmp.getBytes()[i];
                buf[0] = 1;//true creative // false surv
                DatagramPacket req = new DatagramPacket(buf, 257, address, 60010);
                try {
                    socket.send(req);
                } catch (IOException es) {
                    es.printStackTrace();
                }
        
                socket.close();
            }
        }, 600, 600);
    }

    public void onDisable()
    {
        done = true;
    }

    @EventHandler (priority = EventPriority.NORMAL)
    public void ChatEvent(AsyncPlayerChatEvent e)
    {
        DatagramSocket socket = null;

        try {
            socket = new DatagramSocket();
        } catch (SocketException es) {
            es.printStackTrace();
        }

        String msg = e.getPlayer().getName() + ": " + e.getMessage();
        byte[] buf = new byte[257];
        for (int i = 0; i < msg.getBytes().length; i++)
            buf[i + 1] = msg.getBytes()[i];
        buf[0] = 1;//true creative // false surv
        DatagramPacket req = new DatagramPacket(buf, 257, address, 60009);
        try {
            socket.send(req);
        } catch (IOException es) {
            es.printStackTrace();
        }

        socket.close();
    }

    public boolean onCommand(CommandSender sender, Command cmd, String label, String args[])
    {
        return true;
    }

    InetAddress address = null;
    static String msg = "";
    static boolean done = false;
}
