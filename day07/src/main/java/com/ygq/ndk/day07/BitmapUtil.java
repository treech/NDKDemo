package com.ygq.ndk.day07;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;

public class BitmapUtil {

    /**
     * 使用ColorMatrix处理
     *
     * @param src 原图
     * @return 处理后的图
     */
    public static Bitmap gray(Bitmap src) {
        Bitmap dst = Bitmap.createBitmap(src.getWidth(), src.getHeight(), src.getConfig());
        Canvas canvas = new Canvas(dst);

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG);

        ColorMatrix colorMatrix = new ColorMatrix();
        // 原图 参考ColorMatrix的reset方法
//        float[] matrix = {
//                1, 0, 0, 0, 0,
//                0, 1, 0, 0, 0,
//                0, 0, 1, 0, 0,
//                0, 0, 0, 1, 0,
//        };

        // 灰度图 0.11f * r + 0.59f * g + 0.30f * b
        // 参考ColorMatrix的setSaturation方法
//        float[] matrix = {
//                0.11f, 0.59f, 0.30f, 0, 0,
//                0.11f, 0.59f, 0.30f, 0, 0,
//                0.11f, 0.59f, 0.30f, 0, 0,
//                0, 0, 0, 1, 0,
//        };

        // 底片效果 255-r 255-g 255-b
        // 参考ColorMatrix的setSaturation方法
        float[] matrix = {
                -1, 0, 0, 0, 255,
                0, -1, 0, 0, 255,
                0, 0, -1, 0, 255,
                0, 0, 0, 1, 0,
        };
        colorMatrix.set(matrix);
        paint.setColorFilter(new ColorMatrixColorFilter(colorMatrix));
        canvas.drawBitmap(src, 0, 0, paint);
        return dst;
    }

    /**
     * 逐像素处理
     *
     * @param src 原图
     * @return 处理后的图
     */
    public static Bitmap grayV2(Bitmap src) {
        Bitmap dst = Bitmap.createBitmap(src.getWidth(), src.getHeight(), src.getConfig());
        int[] pixels = new int[src.getWidth() * src.getHeight()];
        src.getPixels(pixels, 0, src.getWidth(), 0, 0, src.getWidth(), src.getHeight());
        for (int i = 0; i < pixels.length; i++) {
            int pixel = pixels[i];
            // 获取ARGB
            int a = pixel >> 24 & 0xff;
            int r = pixel >> 16 & 0xff;
            int g = pixel >> 8 & 0xff;
            int b = pixel & 0xff;

            // 灰度图 0.11f * r + 0.59f * g + 0.30f * b
            int gray = (int) (0.11f * r + 0.59f * g + 0.30f * b);

            // 转回像素数据
            pixels[i] = a << 24 | gray << 16 | gray << 8 | gray;
        }
        dst.setPixels(pixels, 0, src.getWidth(), 0, 0, src.getWidth(), src.getHeight());
        return dst;
    }

    /**
     * 仅处理上半部分
     *
     * @param src 原图
     * @return 处理后的图
     */
    public static Bitmap grayV3(Bitmap src) {
        Bitmap dst = Bitmap.createBitmap(src.getWidth(), src.getHeight(), src.getConfig());
        int[] pixels = new int[src.getWidth() * src.getHeight()];
        src.getPixels(pixels, 0, src.getWidth(), 0, 0, src.getWidth(), src.getHeight());
        for (int i = 0; i < pixels.length / 2; i++) {
            int pixel = pixels[i];
            // 获取ARGB
            int a = pixel >> 24 & 0xff;
            int r = pixel >> 16 & 0xff;
            int g = pixel >> 8 & 0xff;
            int b = pixel & 0xff;

            // 灰度图 0.11f * r + 0.59f * g + 0.30f * b
            int gray = (int) (0.11f * r + 0.59f * g + 0.30f * b);

            // 转回像素数据
            pixels[i] = a << 24 | gray << 16 | gray << 8 | gray;
        }
        dst.setPixels(pixels, 0, src.getWidth(), 0, 0, src.getWidth(), src.getHeight());
        return dst;
    }

    /**
     * 仅处理上半部分,没有创建副本，直接修改原图，native层内存复用
     * 需要将isMutable设置为true或Options的属性inMutable为true，才能修改原图的像素，否则会直接抛异常
     *
     * @param src 原图
     */
    public static void grayV4(Bitmap src) {
        int[] pixels = new int[src.getWidth() * src.getHeight()];
        src.getPixels(pixels, 0, src.getWidth(), 0, 0, src.getWidth(), src.getHeight());
        for (int i = 0; i < pixels.length / 2; i++) {
            int pixel = pixels[i];
            // 获取ARGB
            int a = pixel >> 24 & 0xff;
            int r = pixel >> 16 & 0xff;
            int g = pixel >> 8 & 0xff;
            int b = pixel & 0xff;

            // 灰度图 0.11f * r + 0.59f * g + 0.30f * b
            int gray = (int) (0.11f * r + 0.59f * g + 0.30f * b);

            // 转回像素数据
            pixels[i] = a << 24 | gray << 16 | gray << 8 | gray;
        }
        src.setPixels(pixels, 0, src.getWidth(), 0, 0, src.getWidth(), src.getHeight());
    }
}
