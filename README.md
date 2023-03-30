# taas-fargo
Matthias Thimm (matthias.thimm@fernuni-hagen.de)


taas-fargo is a direct solver for abstract argumentation problems and written
in C++.

In order to compile taas-fargo do
```
    make
```
taas-fargo uses a ICCMA23-compliant command line interface, see also
https://iccma2023.github.io/rules.html.

taas-fargo implements the problems [DC-CO,DC-PR,SE-GR,EE-GR,SE-CO,DS-CO] for
correct reasoning and [DC-CO,DC-PR,DC-ST,DC-SST,DC-STG,DC-ID,DS-CO,DS-PR,DS-ST,DS-SST,DS-STG,DS-ID,DS-GR,DC-GR] for
approximative reasoning. For example, to decide
credulous acceptance of an argument wrt. preferred semantics of an AAF in
ICCMA23 format use
```
  taas-fargo -p DC-PR -fo i23 -f <file in ICCMA23 format> -a <argument>
```
For using the approximative version taas-fargo-limited, use the taas-fargo-limited.sh shell skript via, e.g.
```
  taas-fargo-limited.sh -p DC-PR -fo i23 -f <file in ICCMA23 format> -a <argument>
```
