package processing.mode.cpp;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import javax.imageio.ImageIO;

/**
 * The export's icon, from one PNG: an .icns for the macOS .app, an .ico for
 * Windows, and a PNG for Linux (.desktop file and AppImage). Both .icns and
 * .ico hold PNG images inside, which every macOS and Windows since 2009
 * reads, so nothing outside Java is needed to make them.
 */
final class AppIcons {
  private AppIcons() {}

  static BufferedImage read(File png) throws IOException {
    BufferedImage img = ImageIO.read(png);
    if (img == null) throw new IOException(png.getName() + " isn't an image Java can read");
    return img;
  }

  static BufferedImage scale(BufferedImage src, int size) {
    BufferedImage out = new BufferedImage(size, size, BufferedImage.TYPE_INT_ARGB);
    Graphics2D g = out.createGraphics();
    g.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BICUBIC);
    g.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);
    // fit inside the square, centered, keeping the shape
    double k = Math.min((double) size / src.getWidth(), (double) size / src.getHeight());
    int w = (int) Math.round(src.getWidth() * k), h = (int) Math.round(src.getHeight() * k);
    g.drawImage(src, (size - w) / 2, (size - h) / 2, w, h, null);
    g.dispose();
    return out;
  }

  private static byte[] png(BufferedImage img) throws IOException {
    ByteArrayOutputStream b = new ByteArrayOutputStream();
    ImageIO.write(img, "png", b);
    return b.toByteArray();
  }

  static byte[] pngBytes(BufferedImage src, int size) throws IOException {
    return png(scale(src, size));
  }

  static void writePng(BufferedImage src, int size, File dest) throws IOException {
    Files.write(dest.toPath(), png(scale(src, size)));
  }

  /** macOS: ic07..ic10 (128 to 1024 px), each a PNG. */
  static void writeIcns(BufferedImage src, File dest) throws IOException {
    String[] types = { "ic07", "ic08", "ic09", "ic10" };
    int[] sizes = { 128, 256, 512, 1024 };
    ByteArrayOutputStream body = new ByteArrayOutputStream();
    DataOutputStream d = new DataOutputStream(body);
    for (int i = 0; i < types.length; i++) {
      byte[] p = png(scale(src, sizes[i]));
      d.writeBytes(types[i]);
      d.writeInt(p.length + 8);
      d.write(p);
    }
    ByteArrayOutputStream out = new ByteArrayOutputStream();
    DataOutputStream o = new DataOutputStream(out);
    o.writeBytes("icns");
    o.writeInt(body.size() + 8);
    body.writeTo(o);
    Files.write(dest.toPath(), out.toByteArray());
  }

  /** Windows: 16 to 256 px, each a PNG (little-endian header). */
  static void writeIco(BufferedImage src, File dest) throws IOException {
    int[] sizes = { 16, 32, 48, 64, 128, 256 };
    byte[][] imgs = new byte[sizes.length][];
    for (int i = 0; i < sizes.length; i++) imgs[i] = png(scale(src, sizes[i]));
    ByteArrayOutputStream out = new ByteArrayOutputStream();
    le16(out, 0); le16(out, 1); le16(out, sizes.length);
    int offset = 6 + 16 * sizes.length;
    for (int i = 0; i < sizes.length; i++) {
      out.write(sizes[i] >= 256 ? 0 : sizes[i]);
      out.write(sizes[i] >= 256 ? 0 : sizes[i]);
      out.write(0); out.write(0);
      le16(out, 1); le16(out, 32);
      le32(out, imgs[i].length); le32(out, offset);
      offset += imgs[i].length;
    }
    for (byte[] b : imgs) out.write(b);
    Files.write(dest.toPath(), out.toByteArray());
  }

  private static void le16(ByteArrayOutputStream o, int v) { o.write(v & 0xff); o.write((v >> 8) & 0xff); }
  private static void le32(ByteArrayOutputStream o, int v) { le16(o, v & 0xffff); le16(o, (v >> 16) & 0xffff); }
}
