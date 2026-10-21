"""Sets every factory preset's OUTPUT so the louder channel peaks at -1 dBFS (headroom for the mix;
the emergency ceiling at -0.9 dBFS is never reached). Run, rebuild, run again until stable."""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
if os.environ.get("PK_NUMPY_PATH"): sys.path.insert(0, os.environ["PK_NUMPY_PATH"])
import numpy as np
from fit_kicks import Renderer, load_presets
r = Renderer()
p = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "Source", "Presets.cpp")
s = open(p).read()
worst = 0.0
for name, cat, v in load_presets():
    # OUTPUT is linear below the emergency ceiling: probe 24 dB lower and solve exactly.
    pk = 20 * np.log10(np.max(np.abs(r.render({**v, "output": v["output"] - 24.0})))) + 24.0
    worst = max(worst, abs(pk + 1.0))
    m = re.search(r'(\{ "%s",\s+"[^"]+",\s+\{ )([^}]*)( \}(?:, "[^"]*")? \},)' % re.escape(name), s)
    vals = [x.strip() for x in m.group(2).split(",")]
    vals[8] = "%.1ff" % max(-24.0, min(6.0, v["output"] + (-1.0 - pk)))
    s = s[:m.start(2)] + ", ".join(vals) + s[m.end(2):]
open(p, "w").write(s)
r.p.kill()
print("max deviation before this pass: %.2f dB" % worst)
