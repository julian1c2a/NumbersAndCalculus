/**
 * @file test_fixed_precision_types.cpp
 * @brief Prueba de los tipos de precisión fija de Boost.Multiprecision
 * @author AlgoritmiaCombinatoria Project
 * @date 2024
 *
 * Compilar con:
 * g++ -std=c++17 -I../include -DHAS_BOOST_AVAILABLE
 * test_fixed_precision_types.cpp -lboost_system -o test_fixed_precision
 */

#include "../include/multiprecision_io_compatible.hpp"
#include <cassert>
#include <iostream>


using namespace multiprecision_io_compat;

#if HAS_BOOST_MULTIPRECISION
using namespace mp;

void demo_fixed_precision_integers() {
  std::cout << "\n=== DEMOSTRACIÓN TIPOS DE PRECISIÓN FIJA ENTEROS ==="
            << std::endl;

  // Ejemplo con int128_t (tipo de precisión fija de Boost)
  int128_t fixed_128 = int128_t("123456789012345678901234567890123456789");

  std::cout << "int128_t valor: " << to_string(fixed_128) << std::endl;
  std::cout << "int128_t hex: " << to_hex_string(fixed_128) << std::endl;
  std::cout << "int128_t dígitos: " << count_digits(fixed_128) << std::endl;
  std::cout << "int128_t con separadores: "
            << format_with_thousands_separator(fixed_128, ',') << std::endl;

  // Ejemplo con uint256_t
  uint256_t fixed_256 =
      uint256_t("987654321098765432109876543210987654321098765432109876543210");

  std::cout << "\nuint256_t valor: " << to_string(fixed_256) << std::endl;
  std::cout << "uint256_t hex: " << to_hex_string(fixed_256, true, true)
            << std::endl;
  std::cout << "uint256_t dígitos: " << count_digits(fixed_256) << std::endl;
  std::cout << "uint256_t con separadores: "
            << format_with_thousands_separator(fixed_256, '.') << std::endl;

  // Ejemplo con int512_t
  int512_t fixed_512 =
      int512_t("123456789012345678901234567890123456789012345678901234567890123"
               "456789012345678901234567890123456789012345678901234567890123456"
               "789012345678901234567890");

  std::cout << "\nint512_t dígitos: " << count_digits(fixed_512) << std::endl;
  std::cout << "int512_t con separadores (primeros 100 chars): "
            << format_with_thousands_separator(fixed_512, '\'').substr(0, 100)
            << "..." << std::endl;
}

void demo_fixed_precision_floats() {
  std::cout << "\n=== DEMOSTRACIÓN TIPOS DE PRECISIÓN FIJA FLOTANTES ==="
            << std::endl;

  // Ejemplo con cpp_dec_float_50 (50 dígitos decimales)
  cpp_dec_float_50 precise_50(
      "3.14159265358979323846264338327950288419716939937510");

  std::cout << "cpp_dec_float_50 PI: " << to_string(precise_50) << std::endl;
  std::cout << "cpp_dec_float_50 PI (10 decimales): "
            << to_string(precise_50, 10) << std::endl;
  std::cout << "cpp_dec_float_50 PI (20 decimales): "
            << to_string(precise_50, 20) << std::endl;

  // Ejemplo con cpp_dec_float_100
  cpp_dec_float_100 precise_100(
      "2."
      "718281828459045235360287471352662497757247093699959574966967627724076630"
      "353547594571382178525166427427466391932003059921817413596629043572900334"
      "295260595630738132328627943490763233829880753195251019011573834187930702"
      "154089149934884167509244761460668082264800168477411853742345442437107539"
      "077744992069551702761838606261331384583000752044933826560297606737113200"
      "709328709127443747047230696977209310141692836819025515108657463772111252"
      "389784425056953696770785449969967946864454905987931636889230098793127736"
      "178215424999229576351482208269895193668033182528869398496465105820939239"
      "829488793320362509443117301238197068416140397019837679320683282376464804"
      "295311802328782509819455815301756717361332069811250996181881593041690351"
      "598888519345807273866738589422879228499892086805825749279610484198444363"
      "463244968487560233624827041978623209002160990235304369941849146314093431"
      "738143640546253152096183690888707016768396424378140592714563549061303107"
      "208510383750510115747704171898610687396965521267154688957035035402123407"
      "849819334321068170121005627880235193033224745015853904730419957777093503"
      "660416997329725088687696640355570716226844716256079882651787134195124665"
      "201030592123667719432527867539855894489697096409754591856956380234937186"
      "568144972382702340706546080131044575068423300464816140226668084405168451"
      "288615749358859900379632791696150884300440058199520064494069927436490107"
      "810640246096718344854443113959820300921757991809303953956359166677362396"
      "050346520886518436156979623705773026594663483062678426690899821036368993"
      "038066170686860111216286377219365982980746230676590941549130981595226180"
      "966900116063256242262044810849690700810610486425368251908669936188779590"
      "731660270895702143161607022442976865098541039638896860921590231493272260"
      "617002970869697653808430621000769450610970730768547152037360554081510029"
      "529379659525243980963996446474679660097649520850077644060945931000814639"
      "491406447218996447961198430603440399365030849885880066538797369241599696"
      "443989050818661597567797154060870964068535020957091175960924816528710000"
      "560230010818630409595989859590438068716931280006334684569529960000630954"
      "496887723697646535086418797562649361280116351276630157154069786443061133"
      "985296520660060729469468963368169973989660440950273988497235071699780124"
      "569823297345220028270401467157795169880636395020928770419620156238299386"
      "710946009249950531031008960686188835840010517100630063023950201130766813"
      "369694730740064760895060121596483446696759675680666237797563166227176283"
      "984760029968853085686982300506480315293165968161359833970027050009886867"
      "386330514497344566750379369668990265775470408857096556333279457657210204"
      "198710891130587100309018930940639870740064690465035270056182268890377796"
      "479626152978087369837176416234688997652000739066470039593973290120325477"
      "815035987096410198252268258450036338029948548250129948895846064365820960"
      "800063013086596187053290500182986350468063463886448449256158255072038045"
      "775229688995476529685935295936495297643345777360678478639490003023996070"
      "880093997106159675540300137217960086289786886539287979502445401069050588"
      "028029594268210000056380088927420956186414688960809660268638278516958310"
      "710305749674560630940819856066068890238710969983060906410508380655800103"
      "276066888597788515408166186353956060669518631665090239240000500713443258"
      "664258474366713990030899706599938060513167099066514885366241660779069997"
      "01476624508883547424450100000000000000");

  std::cout << "\ncpp_dec_float_100 e: (primeros 50 chars) "
            << to_string(precise_100).substr(0, 50) << "..." << std::endl;
  std::cout << "cpp_dec_float_100 e (15 decimales): "
            << to_string(precise_100, 15) << std::endl;
}

void demo_universal_functions() {
  std::cout << "\n=== DEMOSTRACIÓN FUNCIONES UNIVERSALES ===" << std::endl;

  // Probar que las funciones universales funcionan con tipos de precisión fija
  int256_t big_int(
      "123456789012345678901234567890123456789012345678901234567890");

  std::cout << "Usando universal_to_string: " << universal_to_string(big_int)
            << std::endl;
  std::cout << "Usando universal_to_hex: "
            << universal_to_hex(big_int, true, true) << std::endl;
  std::cout << "Usando universal_count_digits: "
            << universal_count_digits(big_int) << std::endl;
  std::cout << "Usando universal_format_thousands: "
            << universal_format_thousands(big_int, ',') << std::endl;

// Comparar con __int128 si está disponible
#if defined(__GNUC__) || defined(__clang__)
  __int128 native_128 = static_cast<__int128>(123456789012345LL) * 1000000000LL;

  std::cout << "\nComparación con __int128:" << std::endl;
  std::cout << "__int128 universal_to_string: "
            << universal_to_string(native_128) << std::endl;
  std::cout << "__int128 universal_to_hex: "
            << universal_to_hex(native_128, true, true) << std::endl;
  std::cout << "__int128 universal_count_digits: "
            << universal_count_digits(native_128) << std::endl;
  std::cout << "__int128 universal_format_thousands: "
            << universal_format_thousands(native_128, ',') << std::endl;
#endif
}

int main() {
  try {
    show_system_info();

    demo_fixed_precision_integers();
    demo_fixed_precision_floats();
    demo_universal_functions();

    std::cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS EXITOSAMENTE ==="
              << std::endl;

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "❌ Error: " << e.what() << std::endl;
    return 1;
  }
}

#else
int main() {
  std::cout << "❌ Este test requiere Boost.Multiprecision compilado"
            << std::endl;
  std::cout
      << "Compilar con: g++ -std=c++17 -I../include -DHAS_BOOST_AVAILABLE "
         "test_fixed_precision_types.cpp -lboost_system"
      << std::endl;
  return 1;
}
#endif