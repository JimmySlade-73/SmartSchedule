import pdfplumber
import sys

pdf_path = sys.argv[1] if len(sys.argv) > 1 else "horario.pdf"

with pdfplumber.open(pdf_path) as pdf:
    with open("../resources/data/horario.txt", "w", encoding="utf-8") as out:
        for page in pdf.pages:
            text = page.extract_text()
            if text:
                out.write(text + "\n")
            

print("Listo: horario.txt generado")