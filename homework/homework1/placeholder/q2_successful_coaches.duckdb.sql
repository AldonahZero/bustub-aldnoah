SELECT c.name AS coach_name, COUNT(*) AS medal_number
FROM coaches c
         JOIN medals m ON c.country_code = (SELECT country_code FROM teams WHERE code = m.winner_code LIMIT 1)
    AND c.discipline = m.discipline
GROUP BY c.name
ORDER BY medal_number DESC, coach_name ASC;
