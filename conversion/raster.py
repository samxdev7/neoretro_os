import numpy as np
from PIL import Image, ImageDraw, ImageFont
import struct
import os

class SVGARasterizer:
    def __init__(self):
        # Usar la paleta REAL del SVGA que obtuvimos
        self.svga_palette = self._create_real_svga_palette()
        
    def _create_real_svga_palette(self):
        """Usar la paleta REAL del SVGA obtenida del hardware"""
        # Esta es la paleta EXACTA de tu SVGA
        svga_colors = [
            (0, 0, 0), (0, 0, 168), (0, 168, 0), (0, 168, 168),
            (168, 0, 0), (168, 0, 168), (168, 84, 0), (168, 168, 168),
            (84, 84, 84), (84, 84, 252), (84, 252, 84), (84, 252, 252),
            (252, 84, 84), (252, 84, 252), (252, 252, 84), (252, 252, 252),
            (0, 0, 0), (20, 20, 20), (32, 32, 32), (44, 44, 44),
            (56, 56, 56), (68, 68, 68), (80, 80, 80), (96, 96, 96),
            (112, 112, 112), (128, 128, 128), (144, 144, 144), (160, 160, 160),
            (180, 180, 180), (200, 200, 200), (224, 224, 224), (252, 252, 252),
            (0, 0, 252), (64, 0, 252), (124, 0, 252), (188, 0, 252),
            (252, 0, 252), (252, 0, 188), (252, 0, 124), (252, 0, 64),
            (252, 0, 0), (252, 64, 0), (252, 124, 0), (252, 188, 0),
            (252, 252, 0), (188, 252, 0), (124, 252, 0), (64, 252, 0),
            (0, 252, 0), (0, 252, 64), (0, 252, 124), (0, 252, 188),
            (0, 252, 252), (0, 188, 252), (0, 124, 252), (0, 64, 252),
            (124, 124, 252), (156, 124, 252), (188, 124, 252), (220, 124, 252),
            (252, 124, 252), (252, 124, 220), (252, 124, 188), (252, 124, 156),
            (252, 124, 124), (252, 156, 124), (252, 188, 124), (252, 220, 124),
            (252, 252, 124), (220, 252, 124), (188, 252, 124), (156, 252, 124),
            (124, 252, 124), (124, 252, 156), (124, 252, 188), (124, 252, 220),
            (124, 252, 252), (124, 220, 252), (124, 188, 252), (124, 156, 252),
            (180, 180, 252), (196, 180, 252), (216, 180, 252), (232, 180, 252),
            (252, 180, 252), (252, 180, 232), (252, 180, 216), (252, 180, 196),
            (252, 180, 180), (252, 196, 180), (252, 216, 180), (252, 232, 180),
            (252, 252, 180), (232, 252, 180), (216, 252, 180), (196, 252, 180),
            (180, 252, 180), (180, 252, 196), (180, 252, 216), (180, 252, 232),
            (180, 252, 252), (180, 232, 252), (180, 216, 252), (180, 196, 252),
            (0, 0, 112), (28, 0, 112), (56, 0, 112), (84, 0, 112),
            (112, 0, 112), (112, 0, 84), (112, 0, 56), (112, 0, 28),
            (112, 0, 0), (112, 28, 0), (112, 56, 0), (112, 84, 0),
            (112, 112, 0), (84, 112, 0), (56, 112, 0), (28, 112, 0),
            (0, 112, 0), (0, 112, 28), (0, 112, 56), (0, 112, 84),
            (0, 112, 112), (0, 84, 112), (0, 56, 112), (0, 28, 112),
            (56, 56, 112), (68, 56, 112), (84, 56, 112), (96, 56, 112),
            (112, 56, 112), (112, 56, 96), (112, 56, 84), (112, 56, 68),
            (112, 56, 56), (112, 68, 56), (112, 84, 56), (112, 96, 56),
            (112, 112, 56), (96, 112, 56), (84, 112, 56), (68, 112, 56),
            (56, 112, 56), (56, 112, 68), (56, 112, 84), (56, 112, 96),
            (56, 112, 112), (56, 96, 112), (56, 84, 112), (56, 68, 112),
            (80, 80, 112), (88, 80, 112), (96, 80, 112), (104, 80, 112),
            (112, 80, 112), (112, 80, 104), (112, 80, 96), (112, 80, 88),
            (112, 80, 80), (112, 88, 80), (112, 96, 80), (112, 104, 80),
            (112, 112, 80), (104, 112, 80), (96, 112, 80), (88, 112, 80),
            (80, 112, 80), (80, 112, 88), (80, 112, 96), (80, 112, 104),
            (80, 112, 112), (80, 104, 112), (80, 96, 112), (80, 88, 112),
            (0, 0, 64), (16, 0, 64), (32, 0, 64), (48, 0, 64),
            (64, 0, 64), (64, 0, 48), (64, 0, 32), (64, 0, 16),
            (64, 0, 0), (64, 16, 0), (64, 32, 0), (64, 48, 0),
            (64, 64, 0), (48, 64, 0), (32, 64, 0), (16, 64, 0),
            (0, 64, 0), (0, 64, 16), (0, 64, 32), (0, 64, 48),
            (0, 64, 64), (0, 48, 64), (0, 32, 64), (0, 16, 64),
            (32, 32, 64), (40, 32, 64), (48, 32, 64), (56, 32, 64),
            (64, 32, 64), (64, 32, 56), (64, 32, 48), (64, 32, 40),
            (64, 32, 32), (64, 40, 32), (64, 48, 32), (64, 56, 32),
            (64, 64, 32), (56, 64, 32), (48, 64, 32), (40, 64, 32),
            (32, 64, 32), (32, 64, 40), (32, 64, 48), (32, 64, 56),
            (32, 64, 64), (32, 56, 64), (32, 48, 64), (32, 40, 64),
            (44, 44, 64), (48, 44, 64), (52, 44, 64), (60, 44, 64),
            (64, 44, 64), (64, 44, 60), (64, 44, 52), (64, 44, 48),
            (64, 44, 44), (64, 48, 44), (64, 52, 44), (64, 60, 44),
            (64, 64, 44), (60, 64, 44), (52, 64, 44), (48, 64, 44),
            (44, 64, 44), (44, 64, 48), (44, 64, 52), (44, 64, 60),
            (44, 64, 64), (44, 60, 64), (44, 52, 64), (44, 48, 64),
            (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
            (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)
        ]
        
        return svga_colors
    
    def _rgb_to_svga(self, r, g, b):
        """Convertir RGB al color SVGA más cercano usando la paleta real"""
        min_distance = float('inf')
        best_color = 0
        
        for i, (pr, pg, pb) in enumerate(self.svga_palette):
            distance = (r - pr) ** 2 + (g - pg) ** 2 + (b - pb) ** 2
            if distance < min_distance:
                min_distance = distance
                best_color = i
                
        return best_color
    
    def rasterize_image(self, input_image_path, output_bin_path, output_jpg_path):
        """Proceso completo de rasterización con paleta REAL SVGA"""
        print("🔍 Cargando imagen original...")
        original = Image.open(input_image_path).convert('RGB')
        
        # Redimensionar a 320x200 (RESOLUCIÓN EXACTA del SVGA)
        width = int(input("Ingrese la anchura deseada: "))
        height = int(input("Ingrese la altura deseada: "))
        
        resized = original.resize((width, height), Image.Resampling.LANCZOS)
        print(f"📐 Imagen redimensionada a: {width}x{height}")
        
        # Crear imagen rasterizada
        print("🎨 Aplicando paleta SVGA REAL...")
        rasterized = Image.new('P', (width, height))
        
        # Aplanar paleta para PIL
        flat_palette = []
        for color in self.svga_palette:
            flat_palette.extend(color)
        # Completar si es necesario (deberían ser exactamente 768 bytes)
        while len(flat_palette) < 768:
            flat_palette.append(0)
        
        rasterized.putpalette(flat_palette)
        
        # Convertir cada pixel
        pixels = resized.load()
        raster_pixels = []
        
        for y in range(height):
            for x in range(width):
                r, g, b = pixels[x, y]
                svga_color = self._rgb_to_svga(r, g, b)
                rasterized.putpixel((x, y), svga_color)
                raster_pixels.append(svga_color)
        
        # Guardar archivo binario
        print("💾 Guardando archivo binario...")
        self._save_binary_file(output_bin_path, width, height, raster_pixels)
        
        # Guardar JPG modificado
        print("🖼️ Guardando JPG rasterizado...")
        rasterized.convert('RGB').save(output_jpg_path, 'JPEG', quality=90)
        
        print("✅ Proceso completado!")
        print(f"📊 Archivos generados:")
        print(f"   - Binario: {output_bin_path}")
        print(f"   - JPG: {output_jpg_path}")
        print(f"   - Dimensiones: {width}x{height}")
        print(f"   - Paleta: SVGA REAL (256 colores exactos)")
        
        return width, height, raster_pixels
    
    def _save_binary_file(self, filename, width, height, pixels):
        """Guardar archivo binario SIN encabezado (solo pixeles)"""
        with open(filename, 'wb') as f:
            # SOLO guardar pixeles (sin dimensiones)
            f.write(struct.pack(f'{len(pixels)}B', *pixels))
    
    def _create_comparison(self, original, rasterized, output_path):
        """Crear imagen de comparación lado a lado"""
        comparison = Image.new('RGB', (original.width * 2, original.height))
        comparison.paste(original, (0, 0))
        comparison.paste(rasterized.convert('RGB'), (original.width, 0))
        
        # Agregar texto
        try:
            draw = ImageDraw.Draw(comparison)
            font = ImageFont.load_default()
            draw.text((10, 10), "Original 320x200", fill=(255, 255, 255))
            draw.text((original.width + 10, 10), "SVGA Rasterizado", fill=(255, 255, 255))
        except:
            pass
        
        comparison.save(output_path, 'JPEG', quality=95)

# USO DEL PROGRAMA
def main():
    rasterizer = SVGARasterizer()
    
    # Configuración
    file_name = input("Ingrese el nombre del archivo a rasterizar (max 8 caracteres): ")
    extension = input("Ingrese la extension del archivo a rasterizar (jpg/png): ")
    
    input_image = f"{file_name}.{extension}"
    output_bin = f"{file_name}.bin"
    output_jpg = f"{file_name}_raster.{extension}"
    
    # Rasterizar imagen original
    if os.path.exists(input_image):
        width, height, pixels = rasterizer.rasterize_image(
            input_image, output_bin, output_jpg
        )
        
        # Mostrar información
        file_size = os.path.getsize(output_bin)
        print(f"\n📦 Información del archivo binario:")
        print(f"   - Tamaño: {file_size} bytes")
        print(f"   - Pixeles: {width * height}")
        print(f"   - Resolución: {width}x{height}")
        
    else:
        print(f"❌ No se encuentra {input_image}")

if __name__ == "__main__":
    main()