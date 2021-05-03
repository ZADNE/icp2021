# Block Creator
ICP 2021 semestrální projekt
Autor: Tomáš Dubský (xdubsk08)

Použité knihovny (krom Qt):
rapidxml @ http://rapidxml.sourceforge.net/

## Stručný popis:
### Uložení do souboru:
Knihovnu tvoří adresář (přiložena ukázková knihovna "exampleLib").
Podadresáře knihovny tvoří kategorie.
Atomický block je reprezentován XML souborem s koncovkou "*.atom", kompozitní "*.comp", aplikace "*.appl".
### Generování výstupu:
Každý soubor bloku se (podle jeho typu a dat) sestaví na hlavičkový soubor.
Soubor aplikace se sestaví na C++ zdrojový soubor s main funkcí.
Uživatel může bloky sestavovat každý sám nebo se automaticky sestaví všechny závislé bloky při generování aplikace.
Při sestavování aplikace se rovnou zdrojové soubory zkompilují (g++) na výstupní spustitelnou aplikaci.
### Uživatelské rozhraní:
Vlevo je prohlížeč knihovny, vpravo je záložkový editor bloků/aplikací.
Porty se blokům přidávají/odebírají přes pravé tlačítko.
Instance bloků do kompozitních bloků lze přidat tažením z knihovny vlevo.
Porty se propojují klikáním na příslušné tlačítka portů instancí.
Konstantní vstupy/rozpojení spojů je v rozhrnovacím menu vedle tlačítka portu.
### Ukázková knihovna:
Ukázková knihovna obsahuje některé "standardní" bloky,
např.: algebraické a boolovské operace, výstup atd.
Dále obsahuje "uživatelskou" aplikaci ("AsteroidWatcher"), která tyto standardní bloky využívá.
Tato uživatelská aplikace simuluje asteroid letící kolem Země a podle jeho vzdálenosti píše hlášení na standardní výstup.
Při spouštětění uživatelské aplikace doporučuji se zahazovat error výstup (kde je výpis pro každý přesun hodnoty mezi porty).
		
----
Projekt jsem nechtěl dělat sám, ale kolega, se kterým jsem byl domluvený,
mi 2 týdny před termínem řekl, že projekt dělat nebude a že se mám najít někoho jiného,
ale v té době se už nikdo najít nedal.
Rozhodl jsem se tedy, že tu zhruba povolovinu dodělám sám,
ale vzhledem k tomu, že už jsem neměl moc času (souběžně dělám ještě IJA),
tak kvalita kódu šla v některých částech (zejména některé UI třídy) dolů...
Snažil jsem se aplikaci hlavně dokončit tak, aby se vní skutečně dalo udělat vše, co bylo ve specifikaci.
Děkuji za pochoponí