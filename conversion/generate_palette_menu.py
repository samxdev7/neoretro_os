from PIL import Image, ImageDraw, ImageFont
import os

def create_svga_palette_charts():
    # Paleta SVGA completa (256 colores)
    svga_palette = [
        # 0-15: Colores EGA Básicos
        (0, 0, 0), (0, 0, 168), (0, 168, 0), (0, 168, 168),
        (168, 0, 0), (168, 0, 168), (168, 84, 0), (168, 168, 168),
        (84, 84, 84), (84, 84, 252), (84, 252, 84), (84, 252, 252),
        (252, 84, 84), (252, 84, 252), (252, 252, 84), (252, 252, 252),
        
        # 16-31: Escala de Grises
        (0, 0, 0), (20, 20, 20), (32, 32, 32), (44, 44, 44),
        (56, 56, 56), (68, 68, 68), (80, 80, 80), (96, 96, 96),
        (112, 112, 112), (128, 128, 128), (144, 144, 144), (160, 160, 160),
        (180, 180, 180), (200, 200, 200), (224, 224, 224), (252, 252, 252),
        
        # 32-55: Rueda de Colores - Azul a Rojo
        (0, 0, 252), (64, 0, 252), (124, 0, 252), (188, 0, 252),
        (252, 0, 252), (252, 0, 188), (252, 0, 124), (252, 0, 64),
        (252, 0, 0), (252, 64, 0), (252, 124, 0), (252, 188, 0),
        (252, 252, 0), (188, 252, 0), (124, 252, 0), (64, 252, 0),
        (0, 252, 0), (0, 252, 64), (0, 252, 124), (0, 252, 188),
        (0, 252, 252), (0, 188, 252), (0, 124, 252), (0, 64, 252),
        
        # 56-87: Rueda de Colores - Rosa/Magenta
        (124, 124, 252), (156, 124, 252), (188, 124, 252), (220, 124, 252),
        (252, 124, 252), (252, 124, 220), (252, 124, 188), (252, 124, 156),
        (252, 124, 124), (252, 156, 124), (252, 188, 124), (252, 220, 124),
        (252, 252, 124), (220, 252, 124), (188, 252, 124), (156, 252, 124),
        (124, 252, 124), (124, 252, 156), (124, 252, 188), (124, 252, 220),
        (124, 252, 252), (124, 220, 252), (124, 188, 252), (124, 156, 252),
        
        # 88-119: Rueda de Colores - Rosa Claro
        (180, 180, 252), (196, 180, 252), (216, 180, 252), (232, 180, 252),
        (252, 180, 252), (252, 180, 232), (252, 180, 216), (252, 180, 196),
        (252, 180, 180), (252, 196, 180), (252, 216, 180), (252, 232, 180),
        (252, 252, 180), (232, 252, 180), (216, 252, 180), (196, 252, 180),
        (180, 252, 180), (180, 252, 196), (180, 252, 216), (180, 252, 232),
        (180, 252, 252), (180, 232, 252), (180, 216, 252), (180, 196, 252),
        
        # 112-143: Rueda de Colores - Violeta Oscuro
        (0, 0, 112), (28, 0, 112), (56, 0, 112), (84, 0, 112),
        (112, 0, 112), (112, 0, 84), (112, 0, 56), (112, 0, 28),
        (112, 0, 0), (112, 28, 0), (112, 56, 0), (112, 84, 0),
        (112, 112, 0), (84, 112, 0), (56, 112, 0), (28, 112, 0),
        (0, 112, 0), (0, 112, 28), (0, 112, 56), (0, 112, 84),
        (0, 112, 112), (0, 84, 112), (0, 56, 112), (0, 28, 112),
        
        # 136-159: Rueda de Colores - Violeta Medio
        (56, 56, 112), (68, 56, 112), (84, 56, 112), (96, 56, 112),
        (112, 56, 112), (112, 56, 96), (112, 56, 84), (112, 56, 68),
        (112, 56, 56), (112, 68, 56), (112, 84, 56), (112, 96, 56),
        (112, 112, 56), (96, 112, 56), (84, 112, 56), (68, 112, 56),
        (56, 112, 56), (56, 112, 68), (56, 112, 84), (56, 112, 96),
        (56, 112, 112), (56, 96, 112), (56, 84, 112), (56, 68, 112),
        
        # 160-183: Rueda de Colores - Violeta Claro
        (80, 80, 112), (88, 80, 112), (96, 80, 112), (104, 80, 112),
        (112, 80, 112), (112, 80, 104), (112, 80, 96), (112, 80, 88),
        (112, 80, 80), (112, 88, 80), (112, 96, 80), (112, 104, 80),
        (112, 112, 80), (104, 112, 80), (96, 112, 80), (88, 112, 80),
        (80, 112, 80), (80, 112, 88), (80, 112, 96), (80, 112, 104),
        (80, 112, 112), (80, 104, 112), (80, 96, 112), (80, 88, 112),
        
        # 184-207: Rueda de Colores - Azul Oscuro
        (0, 0, 64), (16, 0, 64), (32, 0, 64), (48, 0, 64),
        (64, 0, 64), (64, 0, 48), (64, 0, 32), (64, 0, 16),
        (64, 0, 0), (64, 16, 0), (64, 32, 0), (64, 48, 0),
        (64, 64, 0), (48, 64, 0), (32, 64, 0), (16, 64, 0),
        (0, 64, 0), (0, 64, 16), (0, 64, 32), (0, 64, 48),
        (0, 64, 64), (0, 48, 64), (0, 32, 64), (0, 16, 64),
        
        # 208-231: Rueda de Colores - Azul Medio
        (32, 32, 64), (40, 32, 64), (48, 32, 64), (56, 32, 64),
        (64, 32, 64), (64, 32, 56), (64, 32, 48), (64, 32, 40),
        (64, 32, 32), (64, 40, 32), (64, 48, 32), (64, 56, 32),
        (64, 64, 32), (56, 64, 32), (48, 64, 32), (40, 64, 32),
        (32, 64, 32), (32, 64, 40), (32, 64, 48), (32, 64, 56),
        (32, 64, 64), (32, 56, 64), (32, 48, 64), (32, 40, 64),
        
        # 232-255: Rueda de Colores - Azul Grisáceo
        (44, 44, 64), (48, 44, 64), (52, 44, 64), (60, 44, 64),
        (64, 44, 64), (64, 44, 60), (64, 44, 52), (64, 44, 48),
        (64, 44, 44), (64, 48, 44), (64, 52, 44), (64, 60, 44),
        (64, 64, 44), (60, 64, 44), (52, 64, 44), (48, 64, 44),
        (44, 64, 44), (44, 64, 48), (44, 64, 52), (44, 64, 60),
        (44, 64, 64), (44, 60, 64), (44, 52, 64), (44, 48, 64)
    ]
    
    # Grupos de colores con sus rangos
    color_groups = [
        {"name": "EGA Básicos", "range": (0, 15), "description": "16 colores EGA estándar"},
        {"name": "Escala de Grises", "range": (16, 31), "description": "16 tonos de gris"},
        {"name": "Rueda Azul-Rojo", "range": (32, 55), "description": "Rueda completa de colores"},
        {"name": "Rueda Rosa/Magenta", "range": (56, 79), "description": "Tonos rosas y magenta"},
        {"name": "Rueda Rosa Claro", "range": (80, 103), "description": "Rosas pastel y claros"},
        {"name": "Rueda Violeta Oscuro", "range": (104, 127), "description": "Violetas oscuros"},
        {"name": "Rueda Violeta Medio", "range": (128, 151), "description": "Violetas medios"},
        {"name": "Rueda Violeta Claro", "range": (152, 175), "description": "Violetas claros"},
        {"name": "Rueda Azul Oscuro", "range": (176, 199), "description": "Azules oscuros"},
        {"name": "Rueda Azul Medio", "range": (200, 223), "description": "Azules medios"},
        {"name": "Rueda Azul Grisáceo", "range": (224, 247), "description": "Azules grisáceos"},
        {"name": "Colores Finales", "range": (248, 255), "description": "Últimos 8 colores"}
    ]
    
    # Crear carpeta para las imágenes
    output_dir = "svga_palette_charts"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    print("Generando gráficos de paleta SVGA por grupos...")
    
    # Generar imagen general de referencia
    create_overview_image(svga_palette, color_groups, output_dir)
    
    # Generar imágenes individuales por grupo
    for group in color_groups:
        create_group_image(svga_palette, group, output_dir)
    
    print(f"✅ Todos los gráficos guardados en la carpeta: {output_dir}/")
    print(f"📊 Total de imágenes generadas: {len(color_groups) + 1}")

def create_overview_image(palette, groups, output_dir):
    """Crear imagen general de toda la paleta"""
    img_width = 1
    img_height = 800
    img = Image.new('RGB', (img_width, img_height), (30, 30, 30))
    draw = ImageDraw.Draw(img)
    
    try:
        font_large = ImageFont.truetype("arial.ttf", 24)
        font_medium = ImageFont.truetype("arial.ttf", 14)
        font_small = ImageFont.truetype("arial.ttf", 10)
    except:
        font_large = ImageFont.load_default()
        font_medium = ImageFont.load_default()
        font_small = ImageFont.load_default()
    
    # Título en la parte inferior para no tapar
    title = "PALETA COMPLETA SVGA - 256 COLORES"
    title_width = draw.textlength(title, font=font_large)
    title_x = (img_width - title_width) // 2
    draw.rectangle([title_x - 15, img_height - 50, title_x + title_width + 15, img_height - 10], 
                 fill=(0, 0, 80), outline=(0, 0, 180))
    draw.text((title_x, img_height - 45), title, fill=(255, 255, 255), font=font_large)
    
    # Mostrar todos los colores en cuadrícula
    colors_per_row = 32
    color_size = img_width // colors_per_row
    rows_needed = 256 // colors_per_row
    
    for i, color in enumerate(palette):
        row = i // colors_per_row
        col = i % colors_per_row
        x1 = col * color_size
        y1 = row * color_size
        x2 = x1 + color_size
        y2 = y1 + color_size
        
        draw.rectangle([x1, y1, x2, y2], fill=color)
        
        # Mostrar número en colores claros/oscuros
        text_color = (255, 255, 255) if sum(color) < 384 else (0, 0, 0)
        if color_size > 20:  # Solo mostrar números si hay espacio
            draw.text((x1 + 2, y1 + 2), str(i), fill=text_color, font=font_small)
    
    # Guardar imagen general
    img.save(f"{output_dir}/00_OVERVIEW_SVGA_PALETTE.png", "PNG")
    print("📋 Imagen general creada: 00_OVERVIEW_SVGA_PALETTE.png")

def create_group_image(palette, group, output_dir):
    """Crear imagen detallada para un grupo específico"""
    start, end = group["range"]
    group_colors = palette[start:end + 1]
    group_size = end - start + 1
    
    img_width = 500  # Un poco más ancho para 2 columnas
    img_height = 1100  # Mucho más alto
    
    img = Image.new('RGB', (img_width, img_height), (40, 40, 40))
    draw = ImageDraw.Draw(img)
    
    try:
        font_large = ImageFont.truetype("arial.ttf", 18)
        font_medium = ImageFont.truetype("arial.ttf", 12)
        font_small = ImageFont.truetype("arial.ttf", 10)
    except:
        font_large = ImageFont.load_default()
        font_medium = ImageFont.load_default()
        font_small = ImageFont.load_default()
    
    # Encabezado compacto
    header_y = 10
    group_title = f"GRUPO: {group['name']} (Colores {start}-{end})"
    description = group['description']
    
    draw.text((20, header_y), group_title, fill=(255, 255, 255), font=font_large)
    draw.text((20, header_y + 25), description, fill=(200, 200, 200), font=font_medium)
    
    # Configuración MÁS COMPACTA
    start_y = 60  # Menos espacio después del encabezado
    bar_width = 480  # Más ancho para 2 columnas
    bar_height = 35  # Más compacto
    margin_x = 20
    margin_y = 5    # Menos espacio entre barras
    
    current_x = margin_x
    current_y = start_y
    
    for i, color in enumerate(group_colors):
        color_index = start + i
        
        # Cambiar a segunda columna si es necesario
        if current_y + bar_height > img_height - 40:
            current_x = img_width // 2 + 10
            current_y = start_y
        
        # Barra más compacta
        draw.rectangle([current_x, current_y, current_x + bar_width, current_y + bar_height], 
                     fill=(60, 60, 60), outline=(100, 100, 100))
        
        # Cuadrado de color más pequeño
        color_box_size = 25
        color_box_x = current_x + 5
        color_box_y = current_y + 5
        draw.rectangle([color_box_x, color_box_y, color_box_x + color_box_size, color_box_y + color_box_size], 
                     fill=color, outline=(200, 200, 200))
        
        # Información más compacta
        text_x = color_box_x + color_box_size + 8
        text_y = color_box_y
        
        # Texto más pequeño
        draw.text((text_x, text_y), f"#{color_index:03d}", 
                 fill=(255, 255, 255), font=font_medium)
        draw.text((text_x, text_y + 14), f"RGB: {color[0]:3d},{color[1]:3d},{color[2]:3d}", 
                 fill=(220, 220, 220), font=font_small)
        
        # Hexadecimal compacto
        hex_color = f"#{color[0]:02X}{color[1]:02X}{color[2]:02X}"
        draw.text((text_x + 120, text_y), f"HEX: {hex_color}", 
                 fill=(220, 220, 220), font=font_small)
        
        current_y += bar_height + margin_y
    
    # Pie de página
    footer_text = f"Total: {group_size} colores | {group['name']}"
    draw.text((20, img_height - 25), footer_text, fill=(150, 150, 150), font=font_small)
    
    # Limpiar nombre de archivo
    safe_name = "".join(c for c in group['name'] if c not in ['/', '\\', ':', '*', '?', '"', '<', '>', '|'])
    safe_name = safe_name.replace(' ', '_')
    filename = f"{output_dir}/{start:03d}_{safe_name.upper()}.png"
    
    img.save(filename, "PNG")
    print(f"🎨 Grupo {group['name']}: {filename} ({img_width}x{img_height})")

# Ejecutar el generador
if __name__ == "__main__":
    create_svga_palette_charts()