# Тестовое задание в Crazy Panda

Напишите консольную программу, которая будет считывать модифицированные характеристики предметов.\
Есть инвентарь, в котором хранится несколько предметов разного свойства, которые имеют общие характеристики ( 
**название**, **тип**, **уровень** (_целое число_), **редкость** ( _Common_, _Rare_, _Epic_ ) )\
и числовые <_double_> характеристики, специфичные для каждого типа ( например, **урон** у оружия или **уровень защиты** у
брони ).\
Также есть модификаторы (_Buffs_), которые имеют фильтры по характеристикам предметов. Например "_уровень больше N_"
или "_редкость равна Rare_". Фильтров может быть несколько.

Программа должна заполнять инвентарь, создавать модификаторы и выводить на экран значение модифицированных
характеристик для всех предметов.

##Входные данные:
###Инвентарь:
```
{ident: "axe_01", type: Melee, level: 3, rarity: Common, damage: 12.0, speed: 0.9 },
{ident: "revolver_01", type: Range, level: 5, rarity: Common, damage: 42.0, speed: 2.0 },
{ident: "revolver_02", type: Range, level: 3, rarity: Rare, damage: 50.0, speed: 2.1 },
{ident: "machinegun_01", type: Range, level: 5, rarity: Epic, damage: 83.1, speed: 10.0 },
{ident: "jacket_01", type: Armour, level: 2, rarity: Common, protection: 2.0 },
{ident: "bulletprof_vest_01", type: Armour, level: 5, rarity: Rare, protection: 30.0 }
```
###Модификаторы:
```
{ ident: "eagle_eye", filters: [{"level", ">=", 4}, { "type", Range}], type: DamageBuff, value: 10 },
{ ident: "donatium_steel", filters: [{"rarity", Rare}], type: ProtectionBuff, value: 15 },
{ ident: "rage_drink", filters: [{"type", Range}, {"type", Melee}], type: SpeedBuff, value: 0.4 }
```
_P.S. Рекомендуется использовать C++17 или новее_
_____
Данное тестовое задание выполнено на C++17 (_clang-1300.0.29.30_).\
Используемые зависимости (подгружаются при сборке автоматически):
- [nlohmann/json](https://github.com/nlohmann/json.git)
- [GoogleTest](https://github.com/google/googletest.git)

Сборка осуществляется через [CMake 3.20+](https://cmake.org/), Unit тесты написаны на фреймворке GoogleTest.

### Инструкция по сборке и запуску:
```shell
# Сборка
mkdir build
cd build
cmake .. && make

#Запуск тестов
cd src
../tests/InventoryTests
```
_**Примечание**: Выполнять вышеуказанные команды из корневой папки проекта._

Выполнил: [Федотенко Николай Владимирович](https://github.com/kalkolay)
