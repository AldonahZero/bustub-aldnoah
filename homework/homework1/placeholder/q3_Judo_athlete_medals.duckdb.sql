-- 选择运动员的名字以及他们获得的奖牌数量
SELECT
    a.name AS athlete_name,                 -- 运动员名字
    COUNT(*) AS medal_number               -- 奖牌总数
FROM
    athletes a
        JOIN
    medals m
    ON a.code = m.winner_code              -- 奖牌的获胜者与运动员匹配
        OR
       a.code IN (                            -- 或者运动员属于获奖团队
           SELECT athletes_code
           FROM teams
           WHERE code = m.winner_code
       )
WHERE
    a.disciplines LIKE '%Judo%'            -- 限制条件：运动员的运动项目包括 Judo
GROUP BY
    a.name                                 -- 按运动员名字分组统计奖牌数量
ORDER BY
    medal_number DESC,                     -- 按奖牌数量降序排序
    athlete_name ASC;                      -- 如果奖牌数量相同，则按名字升序排序
