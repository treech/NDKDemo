package com.ygq.ndk.day07;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;

public class BitmapUtil {

    public static Bitmap gray(Bitmap src) {
        Bitmap dst = Bitmap.createBitmap(src.getWidth(), src.getHeight(), src.getConfig());
        Canvas canvas = new Canvas(dst);

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG);

        ColorMatrix colorMatrix = new ColorMatrix();
        // 原图
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
}
