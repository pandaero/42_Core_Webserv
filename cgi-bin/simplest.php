<!DOCTYPE html>
<html>
  <head>
    <title>PHP Test</title>
  </head>
  <body>
<?php 
$method = getenv('REQUEST_METHOD');
if ($method == 'GET') {
    $query_str = getenv('QUERY_STRING');
    parse_str($query_str, $result);
    echo '    <h2>Hi ' . $result['name'] . ", nice to meet you!</h2>\n";
    if (strcmp($result['answer'], 'yes') === 0 or strcmp($result['answer'], 'y') === 0) {
        echo "    <h3>It's even nicer since you like cats, bc so do we... 😸</h3>\n";
    } else if (strcmp($result['answer'], 'no') === 0 or strcmp($result['answer'], 'n') === 0) {
        echo "    <h3>It's a shame you don't like cats, bc we do... 😾</h3>\n";
    }
    else {
        echo "    <h3>That was not a correct answer, pls do 'yes' or 'no' next time! 😽</h3>\n";
    }
} else if ($method == 'POST') {
    $age = (int)getenv('POST_BODY');
    $facts = array("The concept of age has been contemplated and measured for centuries, but it wasn't until the 18th century that the term 'birthday cake' became popular. Before then, only wealthy individuals celebrated birthdays with elaborate feasts, and it was primarily children's birthdays that were marked with cakes.🎂",
            "Did you know that the age of a grapevine can directly affect the taste of the grapes it produces? As vines age, they often produce grapes that are more concentrated in flavor, making older vines highly prized in the wine industry.🍷",
            "Just like humans, trees have lifespans too. Some of the oldest living organisms on Earth are trees. For example, a Great Basin bristlecone pine named 'Methuselah' is estimated to be over 4,850 years old, making it one of the oldest known living organisms.🌳",
            "The Earth is about 4.5 billion years old – but that’s only a third of the age of the universe – which is 13.5 billion years old!🌌");
    echo '    <h2>Thank you for submitting your age! 🌸 [ ' . $age . " ]</h2>";
    echo "    <h4>Random fun fact...</h4>\n";
    echo "    <p>" . $facts[$age % 4]. "</p>\n";
}

foreach (getenv() as $key => $value) {
	echo $key . ' - ' . $value . '
	';
}
?>
<?php
echo "Content-Type: text/plain\r\n\r\n";

function printSuperglobal($name, $array) {
    echo "Superglobal: $name\n";
    foreach ($array as $key => $value) {
        if (is_array($value)) {
            echo "  $key:\n";
            foreach ($value as $subkey => $subvalue) {
                echo "    [$subkey] => $subvalue\n";
            }
        } else {
            echo "  $key => $value\n";
        }
    }
    echo "\n";
}

printSuperglobal('$_GET', $_GET);
printSuperglobal('$_POST', $_POST);
printSuperglobal('$_COOKIE', $_COOKIE);
printSuperglobal('$_FILES', $_FILES);
printSuperglobal('$_SERVER', $_SERVER);
printSuperglobal('$_ENV', $_ENV);
printSuperglobal('$_REQUEST', $_REQUEST);
printSuperglobal('$_SESSION', $_SESSION);
?>
</body>
</html>
