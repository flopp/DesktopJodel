#include "newAccountDialog.h"
#include "ui_newAccountDialog.h"
#include <QMessageBox>

NewAccountDialog::NewAccountDialog(QWidget* parent) : QDialog(parent)
{
  _ui = new Ui::NewAccountDialog;
  _ui->setupUi(this);

  fill_country_box();

  connect(_ui->deviceUidEdit, SIGNAL(textChanged(QString)), this, SLOT(update_device_uid_label(QString)));
  connect(_ui->generateButton, SIGNAL(clicked()), this, SLOT(create_random_device_uid()));
  connect(_ui->exitButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(_ui->okButton, SIGNAL(clicked()), this, SLOT(validate_and_accept()));

  create_random_device_uid();
  _ui->cityEdit->setText("Berlin");
  _ui->countryBox->setCurrentText("Germany; DE");
  _ui->latEdit->setValue(52.5167);
  _ui->lngEdit->setValue(13.3833);
}


NewAccountDialog::~NewAccountDialog()
{
  delete _ui;
}


QString NewAccountDialog::get_device_uid() const
{
  return _ui->deviceUidEdit->text().toLower();
}


QString NewAccountDialog::get_city() const
{
  return _ui->cityEdit->text();
}


QString NewAccountDialog::get_country_code() const
{
  return _ui->countryBox->currentText().split("; ").last();
}


double NewAccountDialog::get_lat() const
{
  return _ui->latEdit->value();
}


double NewAccountDialog::get_lng() const
{
  return _ui->lngEdit->value();
}


bool NewAccountDialog::validate()
{
  QStringList messages;
  if (_ui->deviceUidEdit->text().size() != 64)
  {
    messages << "The 'Device UID' must contain exactly 64 alpha-numeric characters.";
  }
  if (_ui->cityEdit->text().isEmpty())
  {
    messages << "'City' must not be empty.";
  }

  if (!messages.isEmpty())
  {
    QMessageBox::warning(this, "Errors in Input", messages.join("\n"));
  }

  return messages.isEmpty();
}


void NewAccountDialog::create_random_device_uid()
{
  static QString chars = "abcdefghijklmnopqrstuvwxyz0123456789";
  const int random_string_length = 64;

  QString random_string;
  for (int i = 0; i < random_string_length; ++i)
  {
    const int index = qrand() % chars.length();
    random_string += chars[index];
  }

  _ui->deviceUidEdit->setText(random_string);
}


void NewAccountDialog::validate_and_accept()
{
  if (validate())
  {
    accept();
  }
}


void NewAccountDialog::update_device_uid_label(const QString& device_uid)
{
  _ui->deviceUidLabel->setText(QString("%1/64 characters").arg(device_uid.size()));
}


void NewAccountDialog::fill_country_box()
{
  static QStringList countries = {
      "Afghanistan; AF",
      "Aland Islands; AX",
      "Albania; AL",
      "Algeria; DZ",
      "American Samoa; AS",
      "Andorra; AD",
      "Angola; AO",
      "Anguilla; AI",
      "Antarctica; AQ",
      "Antigua and Barbuda; AG",
      "Argentina; AR",
      "Armenia; AM",
      "Aruba; AW",
      "Ascension Island; AC",
      "Australia; AU",
      "Austria; AT",
      "Azerbaijan; AZ",
      "Bahamas; BS",
      "Bahrain; BH",
      "Barbados; BB",
      "Bangladesh; BD",
      "Belarus; BY",
      "Belgium; BE",
      "Belize; BZ",
      "Benin; BJ",
      "Bermuda; BM",
      "Bhutan; BT",
      "Botswana; BW",
      "Bolivia; BO",
      "Bosnia and Herzegovina; BA",
      "Bouvet Island; BV",
      "Brazil; BR",
      "British Indian Ocean Territory; IO",
      "Brunei Darussalam; BN",
      "Bulgaria; BG",
      "Burkina Faso; BF",
      "Burundi; BI",
      "Cambodia; KH",
      "Cameroon; CM",
      "Canada; CA",
      "Cape Verde; CV",
      "Cayman Islands; KY",
      "Central African Republic; CF",
      "Chad; TD",
      "Chile; CL",
      "China; CN",
      "Christmas Island; CX",
      "Cocos (Keeling) Islands; CC",
      "Colombia; CO",
      "Comoros; KM",
      "Congo; CG",
      "Congo, Democratic Republic; CD",
      "Cook Islands; CK",
      "Costa Rica; CR",
      "Cote D'Ivoire (Ivory Coast); CI",
      "Croatia (Hrvatska); HR",
      "Cuba; CU",
      "Cyprus; CY",
      "Czech Republic; CZ",
      "Denmark; DK",
      "Djibouti; DJ",
      "Dominica; DM",
      "Dominican Republic; DO",
      "East Timor; TP",
      "Ecuador; EC",
      "Egypt; EG",
      "El Salvador; SV",
      "Equatorial Guinea; GQ",
      "Eritrea; ER",
      "Estonia; EE",
      "Ethiopia; ET",
      "European Union; EU",
      "Falkland Islands (Malvinas); FK",
      "Faroe Islands; FO",
      "Fiji; FJ",
      "Finland; FI",
      "France; FR",
      "France, Metropolitan; FX",
      "French Guiana; GF",
      "French Polynesia; PF",
      "French Southern Territories; TF",
      "F.Y.R.O.M. (Macedonia); MK",
      "Gabon; GA",
      "Gambia; GM",
      "Georgia; GE",
      "Germany; DE",
      "Ghana; GH",
      "Gibraltar; GI",
      "Great Britain (UK); GB",
      "Greece; GR",
      "Greenland; GL",
      "Grenada; GD",
      "Guadeloupe; GP",
      "Guam; GU",
      "Guatemala; GT",
      "Guernsey; GG",
      "Guinea; GN",
      "Guinea-Bissau; GW",
      "Guyana; GY",
      "Haiti; HT",
      "Heard and McDonald Islands; HM",
      "Honduras; HN",
      "Hong Kong; HK",
      "Hungary; HU",
      "Iceland; IS",
      "India; IN",
      "Indonesia; ID",
      "Iran; IR",
      "Iraq; IQ",
      "Ireland; IE",
      "Israel; IL",
      "Isle of Man; IM",
      "Italy; IT",
      "Jersey; JE",
      "Jamaica; JM",
      "Japan; JP",
      "Jordan; JO",
      "Kazakhstan; KZ",
      "Kenya; KE",
      "Kiribati; KI",
      "Korea (North); KP",
      "Korea (South); KR",
      "Kuwait; KW",
      "Kyrgyzstan; KG",
      "Laos; LA",
      "Latvia; LV",
      "Lebanon; LB",
      "Liechtenstein; LI",
      "Liberia; LR",
      "Libya; LY",
      "Lesotho; LS",
      "Lithuania; LT",
      "Luxembourg; LU",
      "Macau; MO",
      "Madagascar; MG",
      "Malawi; MW",
      "Malaysia; MY",
      "Maldives; MV",
      "Mali; ML",
      "Malta; MT",
      "Marshall Islands; MH",
      "Martinique; MQ",
      "Mauritania; MR",
      "Mauritius; MU",
      "Mayotte; YT",
      "Mexico; MX",
      "Micronesia; FM",
      "Monaco; MC",
      "Moldova; MD",
      "Mongolia; MN",
      "Montenegro; ME",
      "Montserrat; MS",
      "Morocco; MA",
      "Mozambique; MZ",
      "Myanmar; MM",
      "Namibia; NA",
      "Nauru; NR",
      "Nepal; NP",
      "Netherlands; NL",
      "Netherlands Antilles; AN",
      "Neutral Zone; NT",
      "New Caledonia; NC",
      "New Zealand; NZ",
      "Nicaragua; NI",
      "Niger; NE",
      "Nigeria; NG",
      "Niue; NU",
      "Norfolk Island; NF",
      "Northern Mariana Islands; MP",
      "Norway; NO",
      "Oman; OM",
      "Pakistan; PK",
      "Palau; PW",
      "Palestinian Territory, Occupied; PS",
      "Panama; PA",
      "Papua New Guinea; PG",
      "Paraguay; PY",
      "Peru; PE",
      "Philippines; PH",
      "Pitcairn; PN",
      "Poland; PL",
      "Portugal; PT",
      "Puerto Rico; PR",
      "Qatar; QA",
      "Reunion; RE",
      "Romania; RO",
      "Russian Federation; RU",
      "Rwanda; RW",
      "S. Georgia and S. Sandwich Isls.; GS",
      "Saint Kitts and Nevis; KN",
      "Saint Lucia; LC",
      "Saint Vincent & the Grenadines; VC",
      "Samoa; WS",
      "San Marino; SM",
      "Sao Tome and Principe; ST",
      "Saudi Arabia; SA",
      "Senegal; SN",
      "Serbia; RS",
      "Seychelles; SC",
      "Sierra Leone; SL",
      "Singapore; SG",
      "Slovenia; SI",
      "Slovak Republic; SK",
      "Solomon Islands; SB",
      "Somalia; SO",
      "South Africa; ZA",
      "Spain; ES",
      "Sri Lanka; LK",
      "St. Helena; SH",
      "St. Pierre and Miquelon; PM",
      "Sudan; SD",
      "Suriname; SR",
      "Svalbard & Jan Mayen Islands; SJ",
      "Swaziland; SZ",
      "Sweden; SE",
      "Switzerland; CH",
      "Syria; SY",
      "Taiwan; TW",
      "Tajikistan; TJ",
      "Tanzania; TZ",
      "Thailand; TH",
      "Togo; TG",
      "Tokelau; TK",
      "Tonga; TO",
      "Trinidad and Tobago; TT",
      "Tunisia; TN",
      "Turkey; TR",
      "Turkmenistan; TM",
      "Turks and Caicos Islands; TC",
      "Tuvalu; TV",
      "Uganda; UG",
      "Ukraine; UA",
      "United Arab Emirates; AE",
      "United Kingdom; UK",
      "United States; US",
      "US Minor Outlying Islands; UM",
      "Uruguay; UY",
      "Uzbekistan; UZ",
      "Vanuatu; VU",
      "Vatican City State; VA",
      "Venezuela; VE",
      "Viet Nam; VN",
      "British Virgin Islands; VG",
      "Virgin Islands (U.S.); VI",
      "Wallis and Futuna Islands; WF",
      "Western Sahara; EH",
      "Yemen; YE",
      "Zambia; ZM",
      "Zimbabwe; ZW"
  };

  _ui->countryBox->clear();
  _ui->countryBox->addItems(countries);
}
