-- 找到金牌数量进步最大的 5 个国家以及它们的全女性团队
WITH gold_medal_progress AS (
    SELECT
        c.code AS country_code,                      -- 国家代码
        (SUM(CASE WHEN m.medal_code = 1 THEN 1 ELSE 0 END) -
         SUM(CASE WHEN tm.gold_medal IS NOT NULL THEN tm.gold_medal ELSE 0 END))
               AS increased_gold_medals                   -- 计算金牌增长量
    FROM
        countries c
            LEFT JOIN
        medals m
        ON c.code = (SELECT country_code FROM teams WHERE code = m.winner_code LIMIT 1)
    LEFT JOIN
    tokyo_medals tm
ON c.code = tm.country_code                 -- 将国家与东京数据匹配
GROUP BY
    c.code
ORDER BY
    increased_gold_medals DESC
    LIMIT 5                                         -- 限制为前 5 个国家
    )
SELECT
    gmp.country_code,                               -- 国家代码
    gmp.increased_gold_medals,                     -- 金牌增长量
    t.code AS team_code                            -- 团队代码
FROM
    gold_medal_progress gmp
        JOIN
    teams t
    ON t.country_code = gmp.country_code           -- 筛选对应国家的团队
WHERE
    NOT EXISTS (                                   -- 确保团队全是女性
        SELECT 1
        FROM athletes a
        WHERE a.code = t.athletes_code AND a.gender != 1
    )
ORDER BY
    gmp.increased_gold_medals DESC,                -- 按金牌增长量降序
    gmp.country_code ASC,                          -- 如果增长量相同，按国家代码
